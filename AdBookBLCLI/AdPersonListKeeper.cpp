// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "stdafx.h"
#include "AdPersonListKeeper.h"
/*
Copyright (C) 2015-2017 Goncharov Andrei.

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
namespace adbookcli
{
using namespace System;

AdPersonListKeeper::AdPersonListKeeper():
    _adpList(new adbook::AdPersonDescList())
{
    GC::KeepAlive(this);
}

void AdPersonListKeeper::Add(AdPerson^ adp)
{
    _adpList->push_back(adp->ToUnderlyingType());
    GC::KeepAlive(this);
}

void AdPersonListKeeper::Save() {
    _adpList->Save();
    GC::KeepAlive(this);
}

void AdPersonListKeeper::Load() {
    _adpList->Load();
    GC::KeepAlive(this);
}

AdPersonListKeeper::~AdPersonListKeeper() {
	CleanupUnmanaged();
    GC::KeepAlive(this);
}

AdPersonListKeeper::!AdPersonListKeeper()
{
	FINALISER_CALL_WARNING;
	CleanupUnmanaged();
}

void AdPersonListKeeper::CleanupUnmanaged()
{
	delete _adpList;
	_adpList = nullptr;
}

bool AdPersonListKeeper::MoveNext()
{
    if (_adpList->empty()) {
        return false;
    }
    if (-1 == _itemIndex) {
        _itemIndex = 0;
        return true;
    }
    if (boost::numeric_cast<size_t>(_itemIndex) >= _adpList->size() - 1) {
        return false;
    }
    ++_itemIndex;
    return true;
}

void AdPersonListKeeper::Reset() {
    _itemIndex = -1;
}

Collections::IEnumerator ^ AdPersonListKeeper::IEnumerable_GetEnumerator() {
    return GetEnumerator();    
}

IEnumerator<adbookcli::AdPerson ^> ^ AdPersonListKeeper::GetEnumerator() {
    return this;    
}

System::Object ^ AdPersonListKeeper::IEnumerator_Current::get() {
    return AdPersonListKeeper::Current::get();
}

adbookcli::AdPerson ^ AdPersonListKeeper::Current::get()
{
    if (_itemIndex < 0 || _itemIndex >= _adpList->size()) {
        throw gcnew System::InvalidOperationException();
    }
    AdPerson ^ adp = gcnew AdPerson(_adpList->at(_itemIndex));
    GC::KeepAlive(this);
    return adp;
}

}   // namespace adbookcli

