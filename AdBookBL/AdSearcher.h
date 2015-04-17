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

#pragma once
#include "AdBookBLExport.h"
#include "AdPersonDesc.h"
#include "LdapRequest.h"
#include "AdConnector.h"

namespace adbook
{

class ADBOOKBL_API AdSearcher
{
public:
    AdSearcher(const ConnectionParams & cs);
    ~AdSearcher();

    AdSearcher(const AdSearcher &) = delete;
    AdSearcher(AdSearcher &&) = delete;
    void operator = (const AdSearcher &) = delete;
    void operator = (AdSearcher &&) = delete;

    using OnNewItem = std::function<void(AdPersonDesc && item)>; // new item has been found.
    using OnStop = std::function<void()>;   // SearchThread is about to be completed.
    using OnStart = std::function<void()>;  // SearchThread has just been started.

    void SetCallbacks(const OnNewItem & onNewItem, const OnStart & onStart, const OnStop & onStop);
    void Start(const LdapRequest & ldapRequest);
    bool IsStarted() const;
    void Stop();
    void Wait();

private:
    void ThreadProc(ConnectionParams cs, LdapRequest ldapRequest);
    void IterateThroughSearchResults(IDirectorySearchPtr & dsp, ADS_SEARCH_HANDLE & searchHandle, std::vector<WcharBuf> & attrNames);
    void ReadNextEntry(IDirectorySearchPtr & dsp, ADS_SEARCH_HANDLE & searchHandle, std::vector<WcharBuf> & attrNames);
    void SetCancelationHandle(const IDirectorySearchPtr & dsp, const ADS_SEARCH_HANDLE & searchHandle);
private:
    IDirectorySearchPtr dsp_;   // to cancel async search
    ADS_SEARCH_HANDLE searchHandle_ = nullptr;  // to cancel async search
    std::recursive_mutex searchHandleMutex_;  // sync access to searchHandle_

    std::atomic<bool> stopFlag_ = false;    // to stop searching
    std::future<void> sr_; // search activity monitoring
    const ConnectionParams & cs_;
    OnNewItem onNewItem_;
    OnStart onStart_;
    OnStop onStop_;
};

}