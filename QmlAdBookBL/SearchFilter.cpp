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
#include "SearchFilter.h"

namespace qmladbook
{
    SearchFilter::SearchFilter(SearchFilterType type, SearchFilterRule rule, const QString & value)
        : _type{ type }, _rule{ rule }, _value{ value }
    {
    }

    SearchFilterType SearchFilter::GetType() const
    {
        return _type;
    }

    SearchFilterRule SearchFilter::GetRule() const
    {
        return _rule;
    }

    QString SearchFilter::GetValue() const
    {
        return _value;
    }

    void SearchFilter::SetType(const SearchFilterType & type)
    {
        _type = type;
    }

    void SearchFilter::SetFilterRule(const SearchFilterRule & rule)
    {
        _rule = rule;
    }

    void SearchFilter::SetValue(const QString & value)
    {
        _value = value;
    }

    bool SearchFilter::operator == (const SearchFilter & that) const
    {
        const bool typeEqual = _type.GetId() == that.GetType().GetId();
        const bool ruleEqual = _rule.GetId() == that.GetRule().GetId();
        const bool valueEqual = _value == that.GetValue();
        return typeEqual && ruleEqual && valueEqual;
    }
}
