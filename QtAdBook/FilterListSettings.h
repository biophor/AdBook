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
#pragma once
#include "FilterShared.h"



class FilterEntry
{
public:
    FilterEntry(
        CompositeFilterId filterId,
        const QString & filterValue,
        FilterCondition condition
    )
        : _filterValue{ filterValue },
        _condition{ condition }, _filterCode {filterId}
    {
    }

    FilterEntry(
        adbook::Attributes::AttrId filterId,
        const QString & filterValue,
        FilterCondition condition
    )
        : _filterValue{ filterValue },
        _condition{ condition }, _filterCode{filterId}
    {        
    }

    FilterEntry(
        std::variant<adbook::Attributes::AttrId, CompositeFilterId> filterCode,
        const QString & filterValue,
        FilterCondition condition
    )
        : _filterValue{ filterValue },
        _condition{ condition }, _filterCode{ filterCode }
    {                
    }

    FilterEntry(const FilterEntry & entry) = default;
    FilterEntry & operator = (const FilterEntry & entry) = default;    

    FilterCode GetFilterCode() const {
        return _filterCode;
    }
    FilterType GetFilterType() const {
        if (std::get_if<CompositeFilterId>(&_filterCode)) {
            return FilterType::Composite;
        }
        else if (std::get_if<adbook::Attributes::AttrId>(&_filterCode)) {
            return FilterType::LdapAttr;
        }
        else {
            throw adbook::HrError(E_FAIL, L"invalid Filtertype", __FUNCTIONW__);
        }
    }
    FilterCondition GetCondition() const {
        return _condition;
    }
    QString GetValue() const {
        return _filterValue;
    }
private:    
    FilterCondition _condition;
    QString _filterValue;
    FilterCode _filterCode;
};

class FilterListSettings
{
public:    
    void AddFilter(const FilterEntry & entry) {
        _filters.push_back(entry);
    }
    const FilterEntry & GetFilter(size_t filterIndex) const {
        return _filters.at(filterIndex);
    }
    size_t GetNumFilters() const noexcept {
        return _filters.size();
    }
private:
    std::vector<FilterEntry> _filters;
};

