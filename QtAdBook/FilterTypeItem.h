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

#ifndef FILTERTYPEITEM_H
#define FILTERTYPEITEM_H

#include "FilterShared.h"

class FilterTypeItem : public QStandardItem
{
public:    
    FilterTypeItem(adbook::Attributes::AttrId attrId)
        : QStandardItem(QString::fromStdWString(adbook::Attributes::GetInstance().GetUiAttrName(attrId))),
        _filterType{ FilterType::LdapAttr }, _filterCode{ attrId }
    { }
    FilterTypeItem(CompositeFilterId filterId)
        : QStandardItem(GetFilterUiName(filterId)), 
        _filterType(FilterType::Composite), _filterCode{ filterId }
    { }
    virtual FilterTypeItem * clone() const {
        return new FilterTypeItem(*this);
    }
    int GetFilterCode() const {
        return static_cast<int>(std::get<adbook::Attributes::AttrId>(_filterCode));
    }
    std::variant< adbook::Attributes::AttrId, CompositeFilterId> GetFilterCode2() const {
        return _filterCode;
    }
    FilterType GetFilterType() {
        return _filterType;
    }

    QString GetFilterUiName(CompositeFilterId id);

    QString GetFilterUiName(adbook::Attributes::AttrId attrId);

private:
    FilterType _filterType;
    std::variant< adbook::Attributes::AttrId, CompositeFilterId> _filterCode;
};

#endif // FILTERTYPEITEM_H
