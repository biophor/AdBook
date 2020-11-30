// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/*
Copyright (C) 2015-2020 Andrei Goncharov.

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
#include "debug.h"
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
    AdSearcher::OnError onError;
};

AdSearcher::AdSearcher() : _dataPtr{ new AdSearcherData() }
{}

AdSearcher::~AdSearcher() = default;

void AdSearcher::SetCallbacks (
    const OnNewItem & onNewItem,
    const OnStart & onStart,
    const OnStop & onStop
)
{
    if (IsStarted()) {
        HR_ERROR(E_UNEXPECTED);
    }
    _dataPtr->onNewItem = onNewItem;
    _dataPtr->onStart = onStart;
    _dataPtr->onStop = onStop;
}

void AdSearcher::SetCallbacks (
    const OnNewItem & onNewItem,
    const OnStart & onStart,
    const OnStop & onStop,
    const OnError & onError
)
{
    if (IsStarted()) {
        HR_ERROR(E_UNEXPECTED);
    }
    _dataPtr->onNewItem = onNewItem;
    _dataPtr->onStart = onStart;
    _dataPtr->onStop = onStop;
    _dataPtr->onError = onError;
}

void AdSearcher::Start (
    const std::wstring & ldapRequest,
    const ConnectionParams & connectionParams
)
{
    if (IsStarted())
    {
        Stop();
        Wait();
    }
    _dataPtr->connectionParams = connectionParams;
    _dataPtr->dirSearchPtr = nullptr;
    _dataPtr->searchHandle = nullptr;
    _dataPtr->stopFlag = false;

    _dataPtr->searchResultFuture = std::async (
        std::launch::async,
        &AdSearcher::ThreadProc,
        this,
        _dataPtr->connectionParams,
        ldapRequest
    );
}

bool AdSearcher::IsStarted() const
{
    if (_dataPtr->searchResultFuture.valid()) {
        return (_dataPtr->searchResultFuture.wait_for (
            std::chrono::seconds(0)) == std::future_status::timeout
            );
    }
    return false;
}

void AdSearcher::Stop()
{
    if (IsStarted() && !(_dataPtr->stopFlag))
    {
        _dataPtr->stopFlag = true;

        std::lock_guard<std::recursive_mutex> guard(_dataPtr->searchHandleMutex);

        if (_dataPtr->dirSearchPtr && _dataPtr->searchHandle)
        {
            HRESULT hr = _dataPtr->dirSearchPtr->AbandonSearch(_dataPtr->searchHandle);
            if (HRESULT_FROM_WIN32(ERROR_GEN_FAILURE) == hr)
            {
                // not an error.
                return;
            }
            if (FAILED(hr)) {
                throw HrError(hr, L"IDirectorySearch::AbandonSearch() failed.", __FUNCTIONW__);
            }
        }
    }
}

void AdSearcher::Wait()
{
    if (_dataPtr && _dataPtr->searchResultFuture.valid()) {
        _dataPtr->searchResultFuture.wait();
    }
}

void AdSearcher::PropogateSearchException()
{
    if (_dataPtr && _dataPtr->searchResultFuture.valid()) {
        _dataPtr->searchResultFuture.get();
    }
}

void AdSearcher::SetupSearchPrefs( IDirectorySearchPtr & dsp )
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
    if (FAILED(hr)) {
        throw HrError(hr, L"IDirectorySearch::SetSearchPreference() failed.", __FUNCTIONW__);
    }
    for (const auto & i : asi)
    {
        if (ADS_STATUS_S_OK != i.dwStatus)
        {
            throw HrError(E_UNEXPECTED, L"unexpected status.", __FUNCTIONW__);
        }
    }
}

// creating a list of the pointers to the attr names
std::vector<WcharBuf> AdSearcher::CreateAttrListToRetrieve(
    std::vector<wchar_t *> & apv
)
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

    std::for_each(
        av.begin(), av.end(),
        [&apv](WcharBuf & attrName) {
            if (!attrName.empty()) {
                apv.push_back(attrName.data());
            }
        }
    );
    return av;
}

void AdSearcher::ExecuteSearch (
    IDirectorySearchPtr & dsp,
    WcharBuf & searchFilter,
    std::vector<wchar_t *> & attrsToRetrieve,
    ADS_SEARCH_HANDLE & searchHandle
)
{
    HRESULT hr = dsp->ExecuteSearch (
        &searchFilter[0],
        &attrsToRetrieve[0],
        static_cast<DWORD>(attrsToRetrieve.size()),
        &searchHandle
    );
    if (FAILED(hr)) {
        throw HrError( hr, L"ExecuteSearch() failed.", __FUNCTIONW__ );
    }
    if (nullptr == searchHandle) {
        throw HrError(E_UNEXPECTED, L"searchHandle is nullptr.", __FUNCTIONW__);
    }
}

template <class funcT, class T>
class LastWill
{
public:
    LastWill(funcT func, T val) : func_(func), val_(val) { }
    ~LastWill() {
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

void AdSearcher::ReadColumn (
    ADS_SEARCH_COLUMN & col,
    AdPersonDesc & person
)
{
    static auto & attrTraits = Attributes::GetInstance();

    switch (col.dwADsType)
    {
    case ADSTYPE_CASE_IGNORE_STRING:
    case ADSTYPE_CASE_EXACT_STRING:
    case ADSTYPE_PRINTABLE_STRING:
    case ADSTYPE_NUMERIC_STRING:
    case ADSTYPE_DN_STRING:
    {
        if (nullptr == col.pszAttrName) {
            throw HrError(E_UNEXPECTED, L"col.pszAttrName is nullptr.", __FUNCTIONW__);
        }
        if (!wcscmp(col.pszAttrName, AdAttrWritableAttrs))
        {
            AdPersonDesc::AttrIds ais;
            for (size_t i = 0, maxi = static_cast<size_t>(col.dwNumValues); i < maxi; ++i)
            {
                if (attrTraits.IsAttrSupported(col.pADsValues[i].CaseExactString))
                {
                    auto attrId = attrTraits.GetAttrId(col.pADsValues[i].CaseExactString);
                    if (attrId != Attributes::AttrId::Dn) { // Dn can't be modified directly
                        ais.insert(attrId);
                    }
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
                static_cast<size_t>(col.pADsValues[0].OctetString.dwLength)
            );
        }
        person.SetBinaryAttr(col.pszAttrName, std::move(bav));
        break;
    }
    }
}


void AdSearcher::ReadNextEntry (
    IDirectorySearchPtr & dsp,
    ADS_SEARCH_HANDLE & searchHandle,
    std::vector<WcharBuf> & attrNames
)
{
    if (!dsp) {
        throw HrError(E_INVALIDARG, L"dsp is nullptr.", __FUNCTIONW__);
    }
    if (!searchHandle) {
        throw HrError(E_INVALIDARG, L"searchHandle is nullptr.", __FUNCTIONW__);
    }

    AdPersonDesc personDesc;
    for (auto & an : attrNames)
    {
        if (an.empty()) {
            throw HrError(E_INVALIDARG, L"attrNames contains an empty item.", __FUNCTIONW__);
        }
        ADS_SEARCH_COLUMN col = {};
        const HRESULT hr = dsp->GetColumn(searchHandle, &an[0], &col);
        if (E_ADS_COLUMN_NOT_SET == hr) {
            continue;
        }
        if (FAILED(hr)) {
            throw HrError(hr, L"GetColumn() failed.", __FUNCTIONW__);
        }
        auto lastWill = CreateLastWill (
            std::bind(&IDirectorySearch::FreeColumn, dsp, std::placeholders::_1),
            &col
        );
        ReadColumn(col, personDesc);
    }
    if (!personDesc.IsAttributeSet(L"cn")) {
        throw HrError(E_UNEXPECTED, L"CN not found.", __FUNCTIONW__);
    }
    if (_dataPtr->onNewItem) {
        _dataPtr->onNewItem(std::move(personDesc));
    }
}

void AdSearcher::IterateThroughSearchResults (
    IDirectorySearchPtr & dsp,
    ADS_SEARCH_HANDLE & searchHandle,
    std::vector<WcharBuf> & attrNames
)
{
    HRESULT hr = S_OK;
    while (!(_dataPtr->stopFlag) && SUCCEEDED(hr = dsp->GetNextRow(searchHandle)))
    {
        if (S_OK == hr)
        {
            ReadNextEntry(dsp, searchHandle, attrNames);
        }
        else if (S_ADS_NOMORE_ROWS == hr)
        {
            DWORD error = ERROR_SUCCESS;
            wchar_t errorDesc[512], provider[512];
            ADsGetLastError (
                &error,
                errorDesc, _countof(errorDesc),
                provider, _countof(provider)
            );
            if (ERROR_MORE_DATA != error) {
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
            throw HrError(hr, L"IDirectorySearch::GetNextRow() failed.", __FUNCTIONW__);
        }
    }
}

void AdSearcher::SetCancelationHandle (
    const IDirectorySearchPtr & dsp,
    const ADS_SEARCH_HANDLE & searchHandle
)
{
    std::lock_guard<std::recursive_mutex> guard(_dataPtr->searchHandleMutex);
    _dataPtr->dirSearchPtr = dsp;
    _dataPtr->searchHandle = searchHandle;
}

void AdSearcher::ResetCancelationHandle()
{
    std::lock_guard<std::recursive_mutex> guard(_dataPtr->searchHandleMutex);
    _dataPtr->dirSearchPtr = nullptr;
    _dataPtr->searchHandle = nullptr;
}

class AdsSearchHandleCloser final {
    IDirectorySearch * _directorySearch;
    ADS_SEARCH_HANDLE _handle;
public:
    AdsSearchHandleCloser(IDirectorySearch * directorySearch, ADS_SEARCH_HANDLE handle)
        : _directorySearch(directorySearch), _handle(handle)
    { }
    void Close() {
        HRESULT hr = _directorySearch->CloseSearchHandle(_handle);
        if (FAILED(hr)) {
            throw HrError(hr, L"IDirectorySearch::CloseSearchHandle() failed.", __FUNCTIONW__);
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

void AdSearcher::ThreadProc (
    ConnectionParams connectionParams,
    std::wstring ldapRequest
) //-V813
{
    ComAutoInitializer comAutoInitializer;

    try
    {
        if (_dataPtr->onStart) {
            _dataPtr->onStart();
        }

        AdConnector connector;
        connector.Connect(connectionParams);

        IDirectorySearchPtr directorySearchPtr = connector.GetDirectoryObject();
        SetupSearchPrefs(directorySearchPtr);

        WcharBuf searchFilter = ToWcharBuf(ldapRequest);
        if (searchFilter.empty()) {
            throw HrError(E_INVALIDARG, L"searchFilter is empty.", __FUNCTIONW__);
        }

        std::vector<wchar_t *> apv;
        std::vector<WcharBuf> attrListToBeRetrieved = CreateAttrListToRetrieve(apv);
        if (attrListToBeRetrieved.empty()) {
            throw HrError(E_UNEXPECTED, L"attrListToBeRetrieved is empty.", __FUNCTIONW__);
        }
        if (apv.size() != attrListToBeRetrieved.size()) {
            throw HrError(E_UNEXPECTED, L"internal error.", __FUNCTIONW__);
        }

        ADS_SEARCH_HANDLE searchHandle = nullptr;
        ExecuteSearch( directorySearchPtr, searchFilter, apv, searchHandle );

        AdsSearchHandleCloser handleCloser( directorySearchPtr, searchHandle );
        SetCancelationHandle( directorySearchPtr, searchHandle );

        IterateThroughSearchResults( directorySearchPtr, searchHandle, attrListToBeRetrieved );

        if (_dataPtr->onStop) {
            _dataPtr->onStop();
        }
        ResetCancelationHandle();
    }
    catch (const Error & e)
    {
        MY_TRACE(L"%s %s", __FUNCTIONW__, e.What(), e.Where());
        ResetCancelationHandle();
        if (_dataPtr->onError) {
            _dataPtr->onError(e.What());
        }
        if (_dataPtr->onStop) {
            _dataPtr->onStop();
        }
        ResetCancelationHandle();
        throw;
    }
    catch (const std::exception & e)
    {
        MY_TRACE("%s %s", __FUNCTION__, e.what());
        ResetCancelationHandle();
        if (_dataPtr->onError) {
            _dataPtr->onError(ToWstring(e.what()));
        }
        if (_dataPtr->onStop) {
            _dataPtr->onStop();
        }
        throw;
    }
}

}   // namespace adbook
