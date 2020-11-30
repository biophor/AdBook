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
#include "AdPerson.h"
#include "AdAccessFactory.h"
#include "AdPersonDescKeeper.h"

using namespace System;
namespace adbookcli
{

void NativeAdPersonDescKeeperPtr::ReleaseNativeResources()
{
    try {}
    finally{
        // there is no need to do anything(in the current implementation)
        //adbook::AbstractAdPersonDescKeeper * lr =
        //    reinterpret_cast<adbook::AbstractAdPersonDescKeeper *>(handle.ToPointer());
        //delete lr;
        handle = IntPtr::Zero;
    }
}

AdPersonDescKeeper::AdPersonDescKeeper(adbook::AbstractAdPersonDescKeeper * nativeKeeper)
    :_nativeStore(nativeKeeper), _managedStore(gcnew List<AdPerson^>())
{
    GC::KeepAlive(this);
}

void AdPersonDescKeeper::Add(AdPerson^ adp)
{
    _managedStore->Add(adp);
}

void AdPersonDescKeeper::Save()
{
    std::vector<adbook::AdPersonDesc> nativeBuffer;
    for each (AdPerson^ adp in _managedStore) {
        nativeBuffer.push_back(adp->ToUnderlyingType());
    }
    _nativeStore->Save(nativeBuffer);
    GC::KeepAlive(this);
}

void AdPersonDescKeeper::Load()
{
    std::vector<adbook::AdPersonDesc> nativeBuffer;
    _nativeStore->Load(nativeBuffer);
    for (auto & adp : nativeBuffer) {
        _managedStore->Add(gcnew AdPerson(adp));
    }
    GC::KeepAlive(this);
}

void AdPersonDescKeeper::SetNameByConnectionParams(ConnectionParams ^ connectionParams)
{
    _nativeStore->SetNameByConnectionParams(connectionParams->ToUnderlyingType());
    GC::KeepAlive(this);
}

Collections::IEnumerator ^ AdPersonDescKeeper::IEnumerable_GetEnumerator() {
    return GetEnumerator();
}

IEnumerator<adbookcli::AdPerson ^> ^ AdPersonDescKeeper::GetEnumerator() {
    return _managedStore->GetEnumerator();
}

}

