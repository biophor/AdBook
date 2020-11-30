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

#include "qmladbookbl_global.h"

namespace qmladbook
{
    enum class SearchFilterTypeId {
        CommonName, SurName, GivenName, DisplayName,
        Email, Company, Title, Department, EmployeeID,
        WorkPhone, HomePhone, MobilePhone,
        LocalityOrCity, StateOrProvince,
        Names,   // CommonName, SurName, GivenName, DisplayName
        Phones,  // WorkPhone, HomePhone, MobilePhone,
        AnyTextAttribute,
        Invalid
    };

    class QMLADBOOKBL_EXPORT SearchFilterType final
    {
    public:
        SearchFilterType();
        SearchFilterType(SearchFilterTypeId id, QString uiName);
        QString GetUiName() const;
        SearchFilterTypeId GetId() const;

        friend bool operator == (const SearchFilterType & left, const SearchFilterType & right) {
            return left._id == right._id;
        }
    private:
        SearchFilterTypeId _id;
        QString _uiName;
    };
}
