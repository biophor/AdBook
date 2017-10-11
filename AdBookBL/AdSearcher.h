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

#pragma once
#include "AdBookBLExport.h"
#include "AdPersonDesc.h"
#include "LdapRequestBuilder.h"
#include "AdConnector.h"
#include "AbstractAdSearcher.h"

namespace adbook
{

struct AdSearcherData;

class ADBOOKBL_API AdSearcher: public AbstractAdSearcher
{
public:
    AdSearcher();
    virtual ~AdSearcher();

    AdSearcher(const AdSearcher &) = delete;
    AdSearcher(AdSearcher &&) = delete;
    AdSearcher & operator = (const AdSearcher &) = delete;
    AdSearcher & operator = (AdSearcher &&) = delete;

    virtual void SetCallbacks(const OnNewItem & onNewItem, const OnStart & onStart, const OnStop & onStop) override;
    virtual void Start(const std::wstring & ldapRequest, const ConnectionParams & cs) override;
    virtual bool IsStarted() const override;
    virtual void Stop() override;
    virtual void Wait() override;

private:
    void ThreadProc(ConnectionParams cs, std::wstring ldapRequest);

    void IterateThroughSearchResults(
        IDirectorySearchPtr & dsp,
        ADS_SEARCH_HANDLE & searchHandle,
        std::vector<WcharBuf> & attrNames
    );

    void ReadNextEntry(
        IDirectorySearchPtr & dsp,
        ADS_SEARCH_HANDLE & searchHandle,
        std::vector<WcharBuf> & attrNames
    );

    void ReadColumn(ADS_SEARCH_COLUMN & col, AdPersonDesc & person);

    void SetCancelationHandle(const IDirectorySearchPtr & dsp, const ADS_SEARCH_HANDLE & searchHandle);

    void SetupSearchPrefs(IDirectorySearchPtr & dsp);

    std::vector<WcharBuf> CreateAttrListToRetrieve(std::vector<wchar_t *> & apv);

    void ExecuteSearch(
        IDirectorySearchPtr & dsp,
        WcharBuf & searchFilter,
        std::vector<wchar_t *> & attrsToRetrieve,
        ADS_SEARCH_HANDLE & searchHandle
    );
private:
    std::shared_ptr<AdSearcherData> dataPtr_;
};

}

