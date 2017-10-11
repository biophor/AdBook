// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/*
Copyright (C) 2015-2020 Goncharov Andrei.

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
#include "shared.h"
#include "AdSearcher.h"

namespace adbook
{

struct AdSearcherData
{
    IDirectorySearchPtr dirSearchPtr; // to cancel async search
    ADS_SEARCH_HANDLE searchHandle = nullptr; // to cancel async search
    std::recursive_mutex searchHandleMutex; // sync access to searchHandle_
    std::future<void> searchResultFuture; // search activity monitoring
    std::atomic<bool> stopFlag = false; // to stop searching
    ConnectionParams connectionParams;
    AdSearcher::OnNewItem onNewItem;
    AdSearcher::OnStart onStart;
    AdSearcher::OnStop onStop;
};

AdSearcher::AdSearcher() : dataPtr_{ new AdSearcherData() }
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
    dataPtr_->onNewItem = onNewItem;
    dataPtr_->onStart = onStart;
    dataPtr_->onStop = onStop;
}

void AdSearcher::Start(const std::wstring & ldapRequest, const ConnectionParams & connectionParams)
{
    if (IsStarted())
    {
        Stop();
        Wait();
    }
    dataPtr_->connectionParams = connectionParams;
    dataPtr_->dirSearchPtr = nullptr;
    dataPtr_->searchHandle = nullptr;

    dataPtr_->stopFlag = false;
    dataPtr_->searchResultFuture = std::async(std::launch::async, &AdSearcher::ThreadProc, this, dataPtr_->connectionParams, ldapRequest);
}

bool AdSearcher::IsStarted() const
{
    if (dataPtr_->searchResultFuture.valid())
    {
        return (dataPtr_->searchResultFuture.wait_for(std::chrono::seconds(0)) == std::future_status::timeout);
    }
    return false;
}

void AdSearcher::Stop()
{
    if (IsStarted() && !(dataPtr_->stopFlag))
    {
        dataPtr_->stopFlag = true;
        std::lock_guard<std::recursive_mutex> guard(dataPtr_->searchHandleMutex);
        if (dataPtr_->dirSearchPtr && dataPtr_->searchHandle)
        {
            HRESULT hr = dataPtr_->dirSearchPtr->AbandonSearch(dataPtr_->searchHandle);
            if (HRESULT_FROM_WIN32(ERROR_GEN_FAILURE) == hr)
            {
                // not an error.
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
    dataPtr_->searchResultFuture.wait();
}


void AdSearcher::SetupSearchPrefs(IDirectorySearchPtr & dsp)
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
std::vector<WcharBuf> AdSearcher::CreateAttrListToRetrieve(std::vector<wchar_t *> & apv)
{
    apv.clear();
    // several adsi functions require writable INput buffers.
    std::vector<WcharBuf> av = Attributes::GetInstance().GetAdsiComplientAttrNames();

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

void AdSearcher::ExecuteSearch(
    IDirectorySearchPtr & dsp,
    WcharBuf & searchFilter,
    std::vector<wchar_t *> & attrsToRetrieve,
    ADS_SEARCH_HANDLE & searchHandle
)
{
    HRESULT hr = dsp->ExecuteSearch(&searchFilter[0], &attrsToRetrieve[0],
        boost::numeric_cast<DWORD>(attrsToRetrieve.size()), &searchHandle);
    if (FAILED(hr))
    {
        HR_ERROR(hr);
    }
    if (nullptr == searchHandle)
    {
        HR_ERROR(E_UNEXPECTED);
    }
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


void AdSearcher::ReadColumn(ADS_SEARCH_COLUMN & col, AdPersonDesc & person)
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
            for (size_t i = 0, maxi = boost::numeric_cast<size_t>(col.dwNumValues); i < maxi; ++i)
            {
                if (Attributes::GetInstance().IsAttrSupported(col.pADsValues[i].CaseExactString))
                {
                    ais.insert(Attributes::GetInstance().GetAttrId(col.pADsValues[i].CaseExactString));
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
                boost::numeric_cast<size_t>(col.pADsValues[0].OctetString.dwLength)
            );
        }
        person.SetBinaryAttr(col.pszAttrName, std::move(bav));
        break;
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
    if (dataPtr_->onNewItem)
    {
        dataPtr_->onNewItem(std::move(personDesc));
    }
}

void AdSearcher::IterateThroughSearchResults(IDirectorySearchPtr & dsp, ADS_SEARCH_HANDLE & searchHandle,
    std::vector<WcharBuf> & attrNames)
{
    HRESULT hr = S_OK;
    while (!(dataPtr_->stopFlag) && SUCCEEDED(hr = dsp->GetNextRow(searchHandle)))
    {
        if (S_OK == hr)
        {
            //Sleep(100);
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
    std::lock_guard<std::recursive_mutex> guard(dataPtr_->searchHandleMutex);
    dataPtr_->dirSearchPtr = dsp;
    dataPtr_->searchHandle = searchHandle;
}

class AdsSearchHandleCloser final {
    IDirectorySearch * _directorySearch;
    ADS_SEARCH_HANDLE _handle;
public:
    AdsSearchHandleCloser(IDirectorySearch * directorySearch, ADS_SEARCH_HANDLE handle):
        _directorySearch(directorySearch), _handle(handle)
    { }
    void Close() {
        HRESULT hr = _directorySearch->CloseSearchHandle(_handle);
        if (FAILED(hr)) {
            throw HrError(hr, __FUNCTIONW__);
        }
    }
    ~AdsSearchHandleCloser() {
        _directorySearch->CloseSearchHandle(_handle);
    }

    AdsSearchHandleCloser(const AdsSearchHandleCloser &) = delete;
    AdsSearchHandleCloser(AdsSearchHandleCloser&&) = delete;
    AdsSearchHandleCloser& operator = (const AdsSearchHandleCloser &) = delete;
    AdsSearchHandleCloser& operator=(AdsSearchHandleCloser&&) = delete;
};

void AdSearcher::ThreadProc(ConnectionParams connectionParams, std::wstring ldapRequest) //-V813
{
    ComAutoInitializer comAutoInitializer;

    try
    {
        if (dataPtr_->onStart)
        {
            dataPtr_->onStart();
        }

        AdConnector connector;
        connector.Connect(connectionParams);

        IDirectorySearchPtr directorySearchPtr = connector.GetDirectoryObject();
        SetupSearchPrefs(directorySearchPtr);

        WcharBuf searchFilter = ToWcharBuf(ldapRequest);
        BOOST_ASSERT(!searchFilter.empty());

        std::vector<wchar_t *> apv;
        std::vector<WcharBuf> attrListToBeRetrieved = CreateAttrListToRetrieve(apv);
        BOOST_ASSERT((!attrListToBeRetrieved.empty()) && (apv.size() == attrListToBeRetrieved.size()));

        ADS_SEARCH_HANDLE searchHandle = nullptr;
        ExecuteSearch(directorySearchPtr, searchFilter, apv, searchHandle);
        AdsSearchHandleCloser handleCloser(directorySearchPtr, searchHandle);
        SetCancelationHandle(directorySearchPtr, searchHandle);
        IterateThroughSearchResults(directorySearchPtr, searchHandle, attrListToBeRetrieved);
        if (dataPtr_->onStop)
        {
            dataPtr_->onStop();
        }
    }
    catch (const std::exception &)
    {
        if (dataPtr_->onStop)
        {
            dataPtr_->onStop();
        }
        throw;
    }
    SetCancelationHandle(nullptr, nullptr);
}

}   // namespace adbook
