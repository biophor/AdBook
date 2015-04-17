#include "stdafx.h"
#include "error.h"
#include "AdSearcher.h"


AdSearcher::AdSearcher(const ConnectionSettings & cs): cs_(cs)
{
    
}

AdSearcher::~AdSearcher()
{
}

void AdSearcher::SetCallbacks(const OnNewItem & onNewItem, const OnStart & onStart, const OnStop & onStop)
{
    onNewItem_ = onNewItem;
    onStart_ = onStart;
    onStop_ = onStop;
}

void AdSearcher::Start(const LdapRequest & ldapRequest)
{
    if (IsStarted())
    {
        Stop();
    }
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
    stopFlag_ = true;
}

void AdSearcher::Wait()
{    
    sr_.get();    
}

namespace
{
    struct ComSimpleAutoInit
    {
    public:
        ComSimpleAutoInit()
        {
            CoInitialize(NULL);
        }
        ~ComSimpleAutoInit()
        {
            CoUninitialize();
        }
        ComSimpleAutoInit(const ComSimpleAutoInit &) = delete;
        ComSimpleAutoInit(ComSimpleAutoInit &&) = delete;
        void operator = (const ComSimpleAutoInit &) = delete;
        void operator = (ComSimpleAutoInit &&) = delete;
    };

    WcharBuf CreateSearchFilter(const LdapRequest & lr)
    {
        const std::wstring tmp = static_cast<const wchar_t *>(lr.Get());
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
                throw HrError(E_UNEXPECTED);
            }
        }
    }

    std::vector<WcharBuf> CreateAttrListToRetrieve(std::vector<wchar_t *> & apv)
    {
        apv.clear();
        std::vector<WcharBuf> av;
        for (const auto & i : LdapRequest::attrInfo)
        {
            const std::wstring tmp = i.second;
            WcharBuf buf(tmp.begin(), tmp.end());
            buf.push_back(0);
            av.push_back(std::move(buf));
        }
        std::for_each(av.begin(), av.end(),
            [&apv](WcharBuf & attrName) { 
                VERIFY(!attrName.empty()); 
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
            throw HrError(hr);
        }
        if (NULL == searchHandle)
        {
            throw HrError(E_UNEXPECTED);
        }
    }

    class AdsSearchHandleAutoCloser
    {
    public:
        AdsSearchHandleAutoCloser(ADS_SEARCH_HANDLE & searchHandle, IDirectorySearchPtr & dsp): searchHandle_(searchHandle), dsp_(dsp)
        {
            VERIFY(dsp != NULL);
        }
        ~AdsSearchHandleAutoCloser()
        {
            if (dsp_ != NULL && searchHandle_!= NULL)
            {
                dsp_->CloseSearchHandle(searchHandle_);
                searchHandle_ = NULL;
            }
        }
        AdsSearchHandleAutoCloser(const AdsSearchHandleAutoCloser &) = delete;
        AdsSearchHandleAutoCloser(AdsSearchHandleAutoCloser &&) = delete;
        void operator = (const AdsSearchHandleAutoCloser &) = delete;
        void operator = (AdsSearchHandleAutoCloser &&) = delete;
    private:
        ADS_SEARCH_HANDLE & searchHandle_;
        IDirectorySearchPtr & dsp_;
    };

    template <class funcT, class T>
    class LastWill
    {
    public:
        LastWill(funcT func, T val): func_(func), val_(val) { }
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
    
    void ReadColumn(ADS_SEARCH_COLUMN & col, AdSearcher::Item & item)
    {
        switch (col.dwADsType)
        {
        case ADSTYPE_CASE_IGNORE_STRING:
        case ADSTYPE_CASE_EXACT_STRING:
        case ADSTYPE_PRINTABLE_STRING:
        case ADSTYPE_NUMERIC_STRING:
            for (DWORD i = 0; i < col.dwNumValues; ++i)
            {
                VERIFY(col.pszAttrName);
                item.stringAttrs[col.pszAttrName].push_back(col.pADsValues[i].CaseExactString); // see ADSVALUE
            }
            break;
        }
    }
}

void AdSearcher::ReadNextEntry(IDirectorySearchPtr & dsp, ADS_SEARCH_HANDLE & searchHandle,
    std::vector<WcharBuf> & attrNames)
{
    VERIFY(dsp != NULL && searchHandle != NULL);
    std::unique_ptr<Item> itemPtr(new Item);
    for (auto & an : attrNames)
    {
        VERIFY(!an.empty());
        ADS_SEARCH_COLUMN col;
        const HRESULT hr = dsp->GetColumn(searchHandle, &an[0], &col);
        if (E_ADS_COLUMN_NOT_SET == hr)
        {
            continue;
        }
        if (FAILED(hr))
        {
            throw HrError(hr, __FUNCTIONW__);
        }
        auto lastWill = CreateLastWill(std::bind(&IDirectorySearch::FreeColumn, dsp, std::placeholders::_1), &col);
        //auto ac = CreateLastWill(std::bind1st(std::mem_fun(&IDirectorySearch::FreeColumn), dsp), &col);
        ReadColumn(col, *itemPtr);
    }
    if (itemPtr->stringAttrs.find(L"cn") == itemPtr->stringAttrs.end())   // CN is mandatory
    {
        throw HrError(E_UNEXPECTED, __FUNCTIONW__);
    }
    if (onNewItem_)
    {
        onNewItem_(std::move(itemPtr));
    }
}

void AdSearcher::IterateThroughSearchResults(IDirectorySearchPtr & dsp, ADS_SEARCH_HANDLE & searchHandle,
    std::vector<WcharBuf> & attrNames)
{
    if (onStart_)
    {
        onStart_();
    }
    HRESULT hr = S_OK;
    while (SUCCEEDED(hr = dsp->GetNextRow(searchHandle)))
    {
        if (stopFlag_ == true)
        {
            hr = dsp->AbandonSearch(&searchHandle);
            break;
        }
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
        throw HrError(hr);
    }
    if (onStop_)
    {
        onStop_();
    }
}

void AdSearcher::ThreadProc(ConnectionSettings cs, LdapRequest ldapRequest)
{
    ComSimpleAutoInit csai;
    
    AdConnector ac(cs);
    ac.Connect();
    
    IDirectorySearchPtr dsp = ac.GetDirectoryObject();
    SetupSearchPrefs(dsp);
    
    WcharBuf searchFilter = CreateSearchFilter(ldapRequest);
    VERIFY(!searchFilter.empty());
    
    std::vector<wchar_t *> apv;
    std::vector<WcharBuf> av = CreateAttrListToRetrieve(apv);
    VERIFY( (!av.empty()) && (apv.size() == av.size()) );
    
    ADS_SEARCH_HANDLE searchHandle = NULL;
    AdsSearchHandleAutoCloser ashac(searchHandle, dsp);
    ExecuteSearch(dsp, searchFilter, apv, searchHandle);
    
    IterateThroughSearchResults(dsp, searchHandle, av);
}

