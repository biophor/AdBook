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
#include "AdAccessFactory.h"

namespace adbookcli
{

    void NativeAdAccessFactoryPtr::ReleaseNativeResources()
    {
        try {}
        finally{// preventing memory leaks due to ThreadAbortException
            try {
                adbook::AbstractAdAccessFactory * adf = reinterpret_cast<adbook::AbstractAdAccessFactory *>(handle.ToPointer());
                if (adf != nullptr) {
                    delete adf;
                    handle = IntPtr::Zero;
                }
            }
            catch (const adbook::Error & e) {
                MY_TRACE(L"%s %s", __FUNCTIONW__, e.What(), e.Where());
            }
            catch (const std::exception & e) {
                MY_TRACE("%s %s", __FUNCTION__, e.what());
            }
        }
    }

AdAccessFactory::AdAccessFactory(): _nativeFactoryPtr(adbook::CreateAdAccessFactoryRawPtr())
{

}

AdConnector^ AdAccessFactory::CreateConnector()
{
    adbook::AbstractAdConnector * nativeConnectorPtr = _nativeFactoryPtr->CreateConnectorRawPtr();
    AdConnector^ managedConnector = gcnew AdConnector(nativeConnectorPtr);
    return managedConnector;
}

AdSearchTask^ AdAccessFactory::CreateSearchTask(LdapRequestBuilder^ requestBuilder, ConnectionParams^ connectionParams)
{
    adbook::AbstractAdSearcher * nativeSearcherPtr = _nativeFactoryPtr->CreateSearcherRawPtr();
    AdSearchTask::Arguments^ arguments = gcnew AdSearchTask::Arguments(requestBuilder, connectionParams);
    AdSearchTask^ managedSearchTask = gcnew AdSearchTask(nativeSearcherPtr, arguments);
    return managedSearchTask;
}

AdPersonDescKeeper^ AdAccessFactory::GetPersonDescKeeper()
{
    adbook::AbstractAdPersonDescKeeper * nativeKeeperPtr = _nativeFactoryPtr->GetAdPersonDescKeeperRawPtr();
    AdPersonDescKeeper^ managedKeeper = gcnew AdPersonDescKeeper(nativeKeeperPtr);
    return managedKeeper;
}

}   // namespace adbookcli

