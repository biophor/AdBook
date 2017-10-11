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
#include "Attributes.h"

namespace adbook
{
    // LdapRequestBuilder is used to construct search requests to Active Directory
class ADBOOKBL_API LdapRequestBuilder final
{
public:
    LdapRequestBuilder();
    ~LdapRequestBuilder();

    enum MatchingRule
    {
        InvalidMatchingRule = 0, ExactMatch, Contains, BeginWith, EndWith
    };

    void AddRule(const Attributes::AttrId attrId, const MatchingRule rule, const std::wstring & value);
    void AddRule(const std::wstring & attrName, const MatchingRule rule, const std::wstring & value);
    void AddOR();  // before: (x=1)(y=2)    after: (|(x=1)(y=2))
    void AddAND(); // before: (x=1)(y=2)    after: (&(x=1)(y=2))
    void AddNOT(); // before: (x=1)         after: (!(x=1))
    std::wstring Get() const;

    void Clear();

private:
    std::wstring request_;
};

}   // namespace adbook

