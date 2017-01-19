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
#pragma once
#include "AdAttribute.h"

namespace adbookcli
{

public ref class LdapRequest sealed
{
public:
    LdapRequest();
    ~LdapRequest();
    !LdapRequest();

    enum class MathingRule {
        InvalidMatchingRule = adbook::LdapRequest::InvalidMatchingRule, 
        ExactMatch = adbook::LdapRequest::ExactMatch,
        Contains = adbook::LdapRequest::Contains,
        BeginWith = adbook::LdapRequest::BeginWith,
        EndWith = adbook::LdapRequest::EndWith
    };

    void AddRule(AttrId attrId, MathingRule rule, String ^ value);
    void AddRule(String ^ attrName, MathingRule rule, String ^ value);
    void AddOR();  // before: (x=1)(y=2)    after: (|(x=1)(y=2))
    void AddAND(); // before: (x=1)(y=2)    after: (&(x=1)(y=2))
    void AddNOT(); // before: (x=1)         after: (!(x=1))
    String^ AsString();
    void Clear();
    adbook::LdapRequest ToUnderlyingType();
private:
    void CleanupUnmanaged();
    adbook::LdapRequest * _ldapRequest;
};

}   // namespace adbookcli
