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
#include "SearchFilterType.h"

namespace qmladbook
{
    SearchFilterType::SearchFilterType()
        : _id{ SearchFilterTypeId::Invalid }
    {

    }

    SearchFilterType::SearchFilterType(SearchFilterTypeId id, QString uiName) :
        _id{ id }, _uiName{ uiName }
    {

    }


    QString SearchFilterType::GetUiName() const
    {
        return _uiName;
    }

    SearchFilterTypeId SearchFilterType::GetId() const
    {
        return _id;
    }
}
