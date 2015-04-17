#pragma once
#include "LdapRequest.h"
#include "AdConnector.h"

class AdSearcher
{
public:
    
    
    AdSearcher(const ConnectionSettings & cs);
    ~AdSearcher();
    
    AdSearcher(const AdSearcher &) = delete;
    AdSearcher(AdSearcher &&) = delete;
    void operator = (const AdSearcher &) = delete;
    void operator = (AdSearcher &&) = delete;
    
    struct Item
    {
        std::map<CString, std::list<CString>> stringAttrs; // <ldap attrname, attrval>
    };
    typedef std::function<void(std::unique_ptr<Item> itemPtr)> OnNewItem;// new item has been found.
    typedef std::function<void()> OnStop;   // SearchThread is about to be completed.
    typedef std::function<void()> OnStart;   // SearchThread is about to be completed.

    void SetCallbacks(const OnNewItem & onNewItem, const OnStart & onStart, const OnStop & onStop);
    void Start(const LdapRequest & ldapRequest);
    bool IsStarted() const;
    void Stop();
    void Wait();
    
private:
    void ThreadProc(ConnectionSettings cs, LdapRequest ldapRequest);    
    void IterateThroughSearchResults(IDirectorySearchPtr & dsp, ADS_SEARCH_HANDLE & searchHandle, std::vector<WcharBuf> & attrNames);
    void ReadNextEntry(IDirectorySearchPtr & dsp, ADS_SEARCH_HANDLE & searchHandle, std::vector<WcharBuf> & attrNames);
private:
    std::atomic<bool> stopFlag_  = false;
    std::future<void> sr_;    
    const ConnectionSettings & cs_;
    OnNewItem onNewItem_;
    OnStart onStart_;
    OnStop onStop_;
};

