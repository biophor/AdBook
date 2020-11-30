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
#include "SearchFiltersSettings.h"

namespace qmladbook
{
    void SearchSettings::SetSerializedFilters(const QByteArray & serializedFilters) {
        _serializedFilters = serializedFilters;
    }

    QByteArray SearchSettings::GetSerializedFilters() const {
        return _serializedFilters;
    }

    void SearchSettings::AllFilteringRulesMustBeMet(bool all) {
        _allFilteringRulesMustBeMet = all;
    }

    bool SearchSettings::AllFilteringRulesMustBeMet() const {
        return _allFilteringRulesMustBeMet;
    }

    void SearchSettings::EnableSearchFilters(bool enable) {
        _searchFiltersEnabled = enable;
    }

    bool SearchSettings::SearchFiltersEnabled() const {
        return _searchFiltersEnabled;
    }

    bool SearchSettings::SearchFiltersEnabled_DefaultValue() {
        return false;
    }
    bool SearchSettings::AllFilteringRulesMustBeMet_DefaultValue() {
        return false;
    }
}
