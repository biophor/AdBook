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
#include "AdAttributes.h"
#include "LdapRequest.h"
#include "InteropHelpers.h"

namespace adbookcli
{
using System::String;
using System::GC;
using System::Diagnostics::Trace;

LdapRequest::LdapRequest(): _ldapRequest(new adbook::LdapRequest()) 
{ }

LdapRequest::~LdapRequest() {
    CleanupUnmanaged();
    GC::KeepAlive(this);
}

LdapRequest::!LdapRequest()
{
	FINALISER_CALL_WARNING;
    CleanupUnmanaged();
}

void LdapRequest::CleanupUnmanaged()
{
    delete _ldapRequest;
    _ldapRequest = nullptr;
}

void LdapRequest::AddRule(AttrId attrId, MathingRule rule, String ^ value)
{
    _ldapRequest->AddRule(
        AdAttributes::ConvertAttrId(attrId),
        static_cast<adbook::LdapRequest::MathingRule>(rule),
        StringToStdWstring(value)
    );
    GC::KeepAlive(this);
}

void LdapRequest::AddRule(String ^ attrName, MathingRule rule, String ^ value)
{
    _ldapRequest->AddRule(
        StringToStdWstring(attrName),
        static_cast<adbook::LdapRequest::MathingRule>(rule),
        StringToStdWstring(value)
    );
    GC::KeepAlive(this);
}

void LdapRequest::AddOR() {
    _ldapRequest->AddOR();
    GC::KeepAlive(this);
}

void LdapRequest::AddAND() {
    _ldapRequest->AddAND();
    GC::KeepAlive(this);
}

void LdapRequest::AddNOT() {
    _ldapRequest->AddNOT();
    GC::KeepAlive(this);
}

String^ LdapRequest::AsString() {
    auto r = gcnew String(_ldapRequest->Get().c_str());
    GC::KeepAlive(this); //-V779
    return r;
}

void LdapRequest::Clear() {
    _ldapRequest->Clear();
    GC::KeepAlive(this); //-V779
}

adbook::LdapRequest LdapRequest::ToUnderlyingType() {
    return *_ldapRequest;    
}

}   // namespace adbookcli

