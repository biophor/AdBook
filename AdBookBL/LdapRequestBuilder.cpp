// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/*
Copyright (C) 2015-2021 Andrei Goncharov.

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
#include "error.h"
#include "shared.h"
#include "LdapRequestBuilder.h"

namespace adbook
{

void LdapRequestBuilder::AddRule (
    const Attributes::AttrId attrId,
    const MatchingRule rule,
    const std::wstring & value
)
{
    auto & attributes = adbook::Attributes::GetInstance();
    const auto attrName = attributes.GetLdapAttrName(attrId);
    AddRule(attrName, rule, value);
}

void LdapRequestBuilder::AddRule (
    const std::wstring & attrName,
    const MatchingRule rule,
    const std::wstring & value
)
{
    const std::wstring trimmedAttrName = Trim(attrName);
    if (trimmedAttrName.empty()) {
        throw HrError(E_INVALIDARG);
    }
    std::wstring trimmedValue = Trim(value);
    if (!trimmedValue.empty())
    {
        ReplaceAllInPlace(trimmedValue, L"\\", L"\\5c");
        ReplaceAllInPlace(trimmedValue, L"*", L"\\2a");
        ReplaceAllInPlace(trimmedValue, L"/", L"\\2f");
        ReplaceAllInPlace(trimmedValue, L"(", L"\\28");
        ReplaceAllInPlace(trimmedValue, L")", L"\\29");
    }
    switch (rule)
    {
    case Contains:
        trimmedValue = std::wstring(L"*") + trimmedValue + L"*";
        break;
    case BeginWith:
        trimmedValue = trimmedValue + L"*";
        break;
    case EndWith:
        trimmedValue = std::wstring(L"*") + trimmedValue;
        break;
    case ExactMatch:
        // do nothing
        break;
    }
    std::wstring s = std::wstring(L"(") + trimmedAttrName + L"=" + trimmedValue + L")";
    request_ = request_ + s;
}

void LdapRequestBuilder::AddOR()
{
    request_ = std::wstring(L"(|") + request_ + L")";
}

void LdapRequestBuilder::AddAND()
{
    request_ = std::wstring(L"(&") + request_ + L")";
}

void LdapRequestBuilder::AddNOT()
{
    request_ = std::wstring(L"(!") + request_ + L")";
}

std::wstring LdapRequestBuilder::Get() const
{
    return request_;
}

void LdapRequestBuilder::Clear()
{
    request_.clear();
}

void LdapRequestBuilder::AddObjectCategoryRule()
{
    AddRule(L"objectCategory", adbook::LdapRequestBuilder::ExactMatch, L"person");
}

}   // namespace adbook

