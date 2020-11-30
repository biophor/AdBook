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
#include "FilterConditionItem.h"


QString FilterConditionItem::GetDisplayName(adbook::LdapRequestBuilder::MatchingRule ruleId) 
{
    switch (ruleId) {
    case adbook::LdapRequestBuilder::Contains:
        return QObject::tr("contains");
    case adbook::LdapRequestBuilder::BeginWith:
        return QObject::tr("begin with");
    case adbook::LdapRequestBuilder::ExactMatch:
        return QObject::tr("exact match");
    case adbook::LdapRequestBuilder::EndWith:
        return QObject::tr("end with");
    default:
        throw adbook::HrError(E_INVALIDARG, __FUNCTIONW__);
    }
}

