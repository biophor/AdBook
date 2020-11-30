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
#include "Stdafx.h"
#include "AdAttributes.h"
#include "LdapRequestBuilder.h"
#include "InteropHelpers.h"

namespace adbookcli
{
using System::String;
using System::GC;
using System::Diagnostics::Trace;


LdapRequestBuilder::LdapRequestBuilder()
    : _ldapRequestBuilder(new adbook::LdapRequestBuilder())
{
    GC::KeepAlive(this);
}

void LdapRequestBuilder::AddRule(AttrId attrId, MatchingRule rule, String ^ value)
{
    _ldapRequestBuilder->AddRule(
        AdAttributes::ConvertAttrId(attrId),
        static_cast<adbook::LdapRequestBuilder::MatchingRule>(rule),
        StringToStdWstring(value)
    );
    GC::KeepAlive(this);
}

void LdapRequestBuilder::AddRule(String ^ attrName, MatchingRule rule, String ^ value)
{
    _ldapRequestBuilder->AddRule(
        StringToStdWstring(attrName),
        static_cast<adbook::LdapRequestBuilder::MatchingRule>(rule),
        StringToStdWstring(value)
    );
    GC::KeepAlive(this);
}

void LdapRequestBuilder::AddOR() {
    _ldapRequestBuilder->AddOR();
    GC::KeepAlive(this);
}

void LdapRequestBuilder::AddAND() {
    _ldapRequestBuilder->AddAND();
    GC::KeepAlive(this);
}

void LdapRequestBuilder::AddNOT() {
    _ldapRequestBuilder->AddNOT();
    GC::KeepAlive(this);
}

String^ LdapRequestBuilder::AsString() {
    auto r = gcnew String(_ldapRequestBuilder->Get().c_str());
    GC::KeepAlive(this); //-V779
    return r;
}

void LdapRequestBuilder::Clear() {
    _ldapRequestBuilder->Clear();
    GC::KeepAlive(this); //-V779
}

adbook::LdapRequestBuilder LdapRequestBuilder::ToUnderlyingType() {
    return *_ldapRequestBuilder;
}

}   // namespace adbookcli

