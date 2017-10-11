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
#include "AdAttribute.h"
#include "NativeObjectPtr.h"

namespace adbookcli
{

using System::Runtime::CompilerServices::RuntimeHelpers;

ref class NativeLdapRequestBuilderPtr : NativeObjectPtr<adbook::LdapRequestBuilder>
{
public:
    NativeLdapRequestBuilderPtr(adbook::LdapRequestBuilder * p) : NativeObjectPtr<adbook::LdapRequestBuilder>(p) {}

protected:
    virtual void ReleaseNativeResources() override
    {
        try {}
        finally {
            adbook::LdapRequestBuilder * lr = reinterpret_cast<adbook::LdapRequestBuilder *>(handle.ToPointer());
            delete lr;
            handle = IntPtr::Zero;
        }
    }
};

public ref class LdapRequestBuilder sealed
{
public:
    LdapRequestBuilder();

    enum class MatchingRule {
        InvalidMatchingRule = adbook::LdapRequestBuilder::InvalidMatchingRule,
        ExactMatch = adbook::LdapRequestBuilder::ExactMatch,
        Contains = adbook::LdapRequestBuilder::Contains,
        BeginWith = adbook::LdapRequestBuilder::BeginWith,
        EndWith = adbook::LdapRequestBuilder::EndWith
    };

    void AddRule(AttrId attrId, MatchingRule rule, String ^ value);
    void AddRule(String ^ attrName, MatchingRule rule, String ^ value);
    void AddOR();  // before: (x=1)(y=2)    after: (|(x=1)(y=2))
    void AddAND(); // before: (x=1)(y=2)    after: (&(x=1)(y=2))
    void AddNOT(); // before: (x=1)         after: (!(x=1))
    String^ AsString();
    void Clear();
    adbook::LdapRequestBuilder ToUnderlyingType();
private:
    NativeLdapRequestBuilderPtr _ldapRequestBuilder;
};

}   // namespace adbookcli

