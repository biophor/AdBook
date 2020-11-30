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

#pragma once
#include "SearchFilterType.h"
#include "SearchFilterRule.h"
#include "qmladbookbl_global.h"

namespace qmladbook
{
    class QMLADBOOKBL_EXPORT SearchFilter final
    {
    public:
        SearchFilter() = default;
        SearchFilter(SearchFilterType type, SearchFilterRule rule, const QString & value);
        SearchFilterType GetType() const;
        SearchFilterRule GetRule() const;
        QString GetValue() const;
        void SetType(const SearchFilterType & type);
        void SetFilterRule(const SearchFilterRule & rule);
        void SetValue(const QString & value);

        bool operator == (const SearchFilter & that) const;
    private:
        SearchFilterType _type;
        SearchFilterRule _rule;
        QString _value;
    };
}
