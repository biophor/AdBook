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
#include "FilterTypeItem.h"

QString FilterTypeItem::GetFilterUiName(CompositeFilterId id)
{
    switch (id)
    {
    case CompositeFilterId::AnyAttribute:
        return QObject::tr("Any attribute");
    default:
        throw adbook::HrError(E_INVALIDARG, L"id", __FUNCTIONW__);
    }
}

QString FilterTypeItem::GetFilterUiName(adbook::Attributes::AttrId attrId) {
    auto & attributes = adbook::Attributes::GetInstance();
    return QString::fromStdWString(attributes.GetUiAttrName(attrId));
}