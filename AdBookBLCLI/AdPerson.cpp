// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
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
#include "Stdafx.h"
#include "AdPerson.h"
#include "AdAttributes.h"
#include "InteropHelpers.h"

namespace adbookcli
{

using System::GC;

AdPerson::AdPerson():
    _personDescPtr(new adbook::AdPersonDesc())
{

}

AdPerson::~AdPerson()
{    
	CleanupUnmanaged();
    GC::KeepAlive(this);
}

AdPerson::!AdPerson()
{
	FINALISER_CALL_WARNING;
	CleanupUnmanaged();
}

void AdPerson::CleanupUnmanaged()
{
	delete _personDescPtr;
	_personDescPtr = nullptr;
}

bool AdPerson::IsAttributeSet(String ^ ldapAttrName) {
    auto r = _personDescPtr->IsAttributeSet(StringToStdWstring(ldapAttrName));
	GC::KeepAlive(this);
	return r;
}

String ^ AdPerson::StringAttr::get(String ^ attrName) 
{
    if (_cashedStringAttributes.ContainsKey(attrName)) {
        return _cashedStringAttributes[attrName];
    }
    auto r = gcnew String(_personDescPtr->GetStringAttr(StringToStdWstring(attrName)).c_str());
    GC::KeepAlive(this);
	return r;
}

void AdPerson::StringAttr::set(String ^ attrName, String^ value)
{
    _cashedStringAttributes.Remove(attrName);
    _personDescPtr->SetStringAttr (
        StringToStdWstring(attrName), StringToStdWstring(value)
    );
    GC::KeepAlive(this);
}

cli::array<Byte> ^ AdPerson::BinaryAttr::get(String ^ attrName)
{
    if (_cashedBinaryAttributes.ContainsKey(attrName)) {
        return _cashedBinaryAttributes[attrName];
    }
    auto r = StdVectorToCliArray (
        _personDescPtr->GetBinaryAttr(StringToStdWstring(attrName))
    );
    GC::KeepAlive(this);
    return r;
}

void AdPerson::BinaryAttr::set(String ^ attrName, cli::array<Byte> ^ value)
{
    _cashedBinaryAttributes.Remove(attrName);
    _personDescPtr->SetBinaryAttr(
        StringToStdWstring(attrName), CliArrayToStdVector(value)
    );
    GC::KeepAlive(this);
}

AdPerson^ AdPerson::operator = (const adbook::AdPersonDesc & adp)
{
    *_personDescPtr = adp;
    _cashedStringAttributes.Clear();
    _cashedBinaryAttributes.Clear();
    return this;
}

AdPerson^ AdPerson::operator = (adbook::AdPersonDesc && adp)
{
    *_personDescPtr = std::move(adp);
    _cashedStringAttributes.Clear();
    _cashedBinaryAttributes.Clear();
    return this;
}

AdPerson::AdPerson(adbook::AdPersonDesc && adp)
    : _personDescPtr(new adbook::AdPersonDesc(std::move(adp)))
{ }

AdPerson::AdPerson(const adbook::AdPersonDesc & adp)
    : _personDescPtr(new adbook::AdPersonDesc(adp))
{ }

adbook::AdPersonDesc AdPerson::ToUnderlyingType() {
    return *_personDescPtr;
}

bool AdPerson::IsAttributeWritable(AttrId id) {
    auto r = _personDescPtr->IsAttributeWritable(AdAttributes::ConvertAttrId(id));
    GC::KeepAlive(this);
    return r;
}

String ^ AdPerson::Dn::get() {
    auto r = gcnew String(_personDescPtr->GetDn().c_str());
    GC::KeepAlive(this);
    return r;
}

}   // namespace adbookcli

