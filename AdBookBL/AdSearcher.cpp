/*
Copyright (C) 2015 Goncharov Andrei.

This file is part of the 'Active Directory Contact Book'.
'Active Directory Contact Book' is free software: you can redistribute it
and/or modify it under the terms of the GNU General Public License as published
by the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

'Active Directory Contact Book' is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
Public License for more details.

You should have received a copy of the GNU General Public License along with
'Active Directory Contact Book'. If not, see <http://www.gnu.org/licenses/>.
*/

#include "stdafx.h"
#include "error.h"
#include "AdSearcher.h"

namespace adbook
{

AdSearcher::AdSearcher(const ConnectionParams & cs) : cs_(cs) //-V730
{

}

AdSearcher::~AdSearcher()
{
}

void AdSearcher::SetCallbacks(const OnNewItem & onNewItem, const OnStart & onStart, const OnStop & onStop)
{
    if (IsStarted())
    {
        HR_ERROR(E_UNEXPECTED);
    }
    onNewItem_ = onNewItem;
    onStart_ = onStart;
    onStop_ = onStop;
}

void AdSearcher::Start(const LdapRequest & ldapRequest)
{
    if (IsStarted())
    {
        Stop();
        Wait();
    }

    // std::lock_guard<std::recursive_mutex> guard(searchHandleMutex_); // the work thread is not running yet    
    dsp_ = nullptr;
    searchHandle_ = nullptr;

    stopFlag_ = false;
    sr_ = std::async(std::bind(&AdSearcher::ThreadProc, this, cs_, ldapRequest));
}

bool AdSearcher::IsStarted() const
{
    if (sr_.valid())
    {
        return (sr_.wait_for(std::chrono::seconds(0)) == std::future_status::timeout);
    }
    return false;
}

void AdSearcher::Stop()
{
    if (IsStarted() && !stopFlag_)
    {
        stopFlag_ = true;
        std::lock_guard<std::recursive_mutex> guard(searchHandleMutex_);
        if (dsp_ && searchHandle_)
        {
            HRESULT hr = dsp_->AbandonSearch(searchHandle_);
            if (HRESULT_FROM_WIN32(ERROR_GEN_FAILURE) == hr)
            {
                // not an error. GetNextRow is not called.
                return;
            }
            if (FAILED(hr))
            {
                HR_ERROR(hr);
            }
        }
    }
}

void AdSearcher::Wait()
{
    sr_.get();
}

namespace
{
WcharBuf CreateSearchFilter(const LdapRequest & lr)
{
    const std::wstring tmp = lr.Get();
    std::vector<wchar_t> result(tmp.begin(), tmp.end());
    result.push_back(0);
    return result;
}

void SetupSearchPrefs(IDirectorySearchPtr & dsp)
{
    ADSVALUE asyncSearch{};
    asyncSearch.dwType = ADSTYPE_BOOLEAN;
    asyncSearch.Boolean = TRUE;

    ADSVALUE searchScope{};
    searchScope.dwType = ADSTYPE_INTEGER;
    searchScope.Integer = ADS_SCOPE_SUBTREE;

    ADSVALUE pageSize{};
    pageSize.dwType = ADSTYPE_INTEGER;
    pageSize.Integer = 1;

    ADS_SEARCHPREF_INFO asi[] = {
        { ADS_SEARCHPREF_ASYNCHRONOUS, asyncSearch },
        { ADS_SEARCHPREF_SEARCH_SCOPE, searchScope },
        { ADS_SEARCHPREF_PAGESIZE, pageSize }
    };
    const HRESULT hr = dsp->SetSearchPreference(&asi[0], _countof(asi));
    if (FAILED(hr))
    {
        throw HrError(hr);
    }
    for (const auto & i : asi)
    {
        if (ADS_STATUS_S_OK != i.dwStatus)
        {
            HR_ERROR(E_UNEXPECTED);
        }
    }
}

// creating a list of the pointers to the attr names
std::vector<WcharBuf> CreateAttrListToRetrieve(std::vector<wchar_t *> & apv)
{
    apv.clear();
    // some adsi functions requires writable INput buffers.
    std::vector<WcharBuf> av = Attributes::GetInstance().GetWritableLdapAttrNames();

    // AD attr names cannot be changed. So, direct usage its names is reasonable.
    const wchar_t * auxAttrs[] = { AdAttrWritableAttrs, AdAttrDn };
    for (std::wstring s : auxAttrs)
    {
        std::vector<wchar_t> buf(s.begin(), s.end());
        buf.push_back(0);   // trailing zero
        av.push_back(std::move(buf));
    }

    std::for_each(av.begin(), av.end(),
        [&apv](WcharBuf & attrName) {
        BOOST_ASSERT(!attrName.empty());
        apv.push_back(&attrName[0]);
    }
    );
    return av;
}

void ExecuteSearch(IDirectorySearchPtr & dsp, WcharBuf & searchFilter, std::vector<wchar_t *> & attrsToRetrieve,
    ADS_SEARCH_HANDLE & searchHandle)
{
    HRESULT hr = dsp->ExecuteSearch(&searchFilter[0], &attrsToRetrieve[0],
        static_cast<DWORD>(attrsToRetrieve.size()), &searchHandle);
    if (FAILED(hr))
    {
        HR_ERROR(hr);
    }
    if (nullptr == searchHandle)
    {
        HR_ERROR(E_UNEXPECTED);
    }
}

template<typename T, typename... Ts>    // see: Meyers S. 'Effective Modern C++' Item 21
std::unique_ptr<T> make_unique(Ts&&... params)
{
    return std::unique_ptr<T>(new T(std::forward<Ts>(params)...));
}

template <class funcT, class T>
class LastWill
{
public:
    LastWill(funcT func, T val) : func_(func), val_(val) { }
    ~LastWill()
    {
        func_(val_);
    }
private:
    funcT func_;
    T val_;
};

template <class funcT, class T>
auto CreateLastWill(funcT func, T t)
{
    return LastWill<funcT, T>(func, t);
}

void ReadColumn(ADS_SEARCH_COLUMN & col, AdPersonDesc & person)
{
    switch (col.dwADsType)
    {
    case ADSTYPE_CASE_IGNORE_STRING:
    case ADSTYPE_CASE_EXACT_STRING:
    case ADSTYPE_PRINTABLE_STRING:
    case ADSTYPE_NUMERIC_STRING:
    case ADSTYPE_DN_STRING:
    {
        BOOST_ASSERT(col.pszAttrName);
        if (!wcscmp(col.pszAttrName, AdAttrWritableAttrs))
        {
            AdPersonDesc::AttrIds ais;
            const auto & a = Attributes::GetInstance();
            for (DWORD i = 0; i < col.dwNumValues; ++i)
            {
                if (a.IsAttrSupported(col.pADsValues[i].CaseExactString))
                {
                    ais.insert(a.GetAttrId(col.pADsValues[i].CaseExactString));
                }
            }
            person.SetWritableAttributes(std::move(ais));
        }
        else
        {
            person.SetStringAttr(col.pszAttrName, col.pADsValues[0].CaseExactString);
        }
        break;
    }
    case ADSTYPE_OCTET_STRING:
    {
        BinaryAttrVal bav(col.pADsValues[0].OctetString.dwLength);
        if (!bav.empty())
        {
            memcpy_s(&bav[0], bav.size(), col.pADsValues[0].OctetString.lpValue,
                col.pADsValues[0].OctetString.dwLength);
        }
        person.SetBinaryAttr(col.pszAttrName, std::move(bav));
        break;
    }
    }
}
}

void AdSearcher::ReadNextEntry(IDirectorySearchPtr & dsp, ADS_SEARCH_HANDLE & searchHandle,
    std::vector<WcharBuf> & attrNames)
{
    BOOST_ASSERT(dsp != nullptr && searchHandle != nullptr);
    AdPersonDesc personDesc;
    for (auto & an : attrNames)
    {
        BOOST_ASSERT(!an.empty());
        ADS_SEARCH_COLUMN col = {};
        const HRESULT hr = dsp->GetColumn(searchHandle, &an[0], &col);
        if (E_ADS_COLUMN_NOT_SET == hr)
        {
            continue;
        }
        if (FAILED(hr))
        {
            HR_ERROR(hr);
        }
        auto lastWill = CreateLastWill(std::bind(&IDirectorySearch::FreeColumn, dsp, std::placeholders::_1), &col);
        ReadColumn(col, personDesc);
    }
    if (!personDesc.IsAttributeSet(L"cn"))
    {
        HR_ERROR(E_UNEXPECTED);
    }
    if (onNewItem_)
    {
        onNewItem_(std::move(personDesc));
    }
}

void AdSearcher::IterateThroughSearchResults(IDirectorySearchPtr & dsp, ADS_SEARCH_HANDLE & searchHandle,
    std::vector<WcharBuf> & attrNames)
{
    HRESULT hr = S_OK;
    while (!stopFlag_ && SUCCEEDED(hr = dsp->GetNextRow(searchHandle)))
    {
        if (S_OK == hr)
        {
            ReadNextEntry(dsp, searchHandle, attrNames);
        }
        else if (S_ADS_NOMORE_ROWS == hr)
        {
            DWORD error = ERROR_SUCCESS;
            wchar_t errorDesc[512], provider[512];
            ADsGetLastError(&error, errorDesc, _countof(errorDesc), provider, _countof(provider));
            if (ERROR_MORE_DATA != error)
            {
                break;
            }
        }
        else
        {
            break;
        }
    }
    if (FAILED(hr))
    {
        if (HRESULT_FROM_WIN32(ERROR_CANCELLED) != hr)  // ERROR_CANCELED - is not an error
        {
            HR_ERROR(hr);
        }
    }
}

void AdSearcher::SetCancelationHandle(const IDirectorySearchPtr & dsp, const ADS_SEARCH_HANDLE & searchHandle)
{
    std::lock_guard<std::recursive_mutex> guard(searchHandleMutex_);
    dsp_ = dsp;
    searchHandle_ = searchHandle;
}

void AdSearcher::ThreadProc(ConnectionParams cs, LdapRequest ldapRequest)
{
    auto hr = CoInitialize(nullptr);
    if (FAILED(hr))
    {
        HR_ERROR(hr);
    }
    try
    {
        if (onStart_)
        {
            onStart_();
        }

        AdConnector ac(cs);
        ac.Connect();

        IDirectorySearchPtr dsp = ac.GetDirectoryObject();
        SetupSearchPrefs(dsp);

        WcharBuf searchFilter = CreateSearchFilter(ldapRequest);
        BOOST_ASSERT(!searchFilter.empty());

        std::vector<wchar_t *> apv;
        std::vector<WcharBuf> av = CreateAttrListToRetrieve(apv);
        BOOST_ASSERT((!av.empty()) && (apv.size() == av.size()));

        ADS_SEARCH_HANDLE searchHandle = nullptr;
        ExecuteSearch(dsp, searchFilter, apv, searchHandle);
        auto lastWill = CreateLastWill(std::bind(&IDirectorySearch::CloseSearchHandle, dsp, std::placeholders::_1), searchHandle);
        SetCancelationHandle(dsp, searchHandle);
        IterateThroughSearchResults(dsp, searchHandle, av);
        if (onStop_)
        {
            onStop_();
        }
#pragma warning(push)
#pragma warning(disable: 4459)
        BOOST_SCOPE_EXIT(void) {
            CoUninitialize();
        } BOOST_SCOPE_EXIT_END
#pragma warning(pop)
    }
    catch (const std::exception &)
    {
        if (onStop_)
        {
            onStop_();
        }
        throw;
    }
    SetCancelationHandle(nullptr, nullptr);
}

}   // namespace adbook
