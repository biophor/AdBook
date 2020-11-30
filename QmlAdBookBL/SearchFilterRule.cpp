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
#include "SearchFilterRule.h"

namespace qmladbook
{

    SearchFilterRule::SearchFilterRule()
        : _ruleId{ SearchFilterRuleId::Invalid }
    {
    }

    SearchFilterRule::SearchFilterRule(SearchFilterRuleId ruleId, const QString & uiName)
        : _uiName{ uiName }, _ruleId{ ruleId }
    {
    }

    SearchFilterRule::~SearchFilterRule()
    {
    }

    QString SearchFilterRule::GetUiName() const
    {
        return _uiName;
    }

    SearchFilterRuleId SearchFilterRule::GetId() const
    {
        return _ruleId;
    }

}
