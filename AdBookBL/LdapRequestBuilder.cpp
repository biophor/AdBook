// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
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

#include "stdafx.h"
#include "error.h"
#include "LdapRequestBuilder.h"

namespace adbook
{

LdapRequestBuilder::LdapRequestBuilder()
{
}


LdapRequestBuilder::~LdapRequestBuilder()
{
}


void LdapRequestBuilder::AddRule(
    const Attributes::AttrId attrId,
    const MatchingRule rule,
    const std::wstring & value
)
{
    auto & attributes = adbook::Attributes::GetInstance();
    const auto attrName = attributes.GetLdapAttrName(attrId);
    AddRule(attrName, rule, value);
}

void LdapRequestBuilder::AddRule(
    const std::wstring & attrName,
    const MatchingRule rule,
    const std::wstring & value
)
{
    std::wstring lan = attrName;
    boost::trim(lan);
    if (lan.empty())
    {
        throw HrError(E_INVALIDARG);
    }
    std::wstring lv = value;
    boost::trim(lv);
    if (!lv.empty())
    {
        boost::replace_all(lv, L"\\", L"\\5c");
        boost::replace_all(lv, L"*", L"\\2a");
        boost::replace_all(lv, L"/", L"\\2f");
        boost::replace_all(lv, L"\0", L"\\00");
        boost::replace_all(lv, L"(", L"\\28");
        boost::replace_all(lv, L")", L"\\29");
    }
    switch (rule)
    {
    case Contains:
        lv = std::wstring(L"*") + lv + L"*";
        break;
    case BeginWith:
        lv = lv + L"*";
        break;
    case EndWith:
        lv = std::wstring(L"*") + lv;
        break;
    case ExactMatch:
        // do nothing
        break;
    }
    std::wstring s = std::wstring(L"(") + lan + L"=" + lv + L")";
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

}   // namespace adbook

