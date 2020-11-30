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

#pragma once
class MainWndSettings
{
public:
    void SetPreviousSearches(QStringList prevSearchValues) {
        _prevSearchAttrValues = prevSearchValues;
    }
    QStringList GetPrevSearches() const {
        return _prevSearchAttrValues;
    }

    void SetMainWndSplitterState(QList<int> positions) {
        _mainWndSplitterState = positions;
    }
    QList<int> GetMainWndSplitterState() const {
        return _mainWndSplitterState;
    }

    void SetContactsSplitterState(QList<int> positions) {
        _contactSplitterState = positions;
    }
    QList<int> GetContactsSplitterState() const {
        return _contactSplitterState;
    }

    void SetGeometry(QRect geom) {
        _geometry = geom;
    }
    QRect GetGeometry() const {
        return _geometry;
    }

    void SetFlagAllConditionsShouldBeMet(bool flag) {
        _allTheConditionsShouldBeMet = flag;
    }
    bool GetFlagAllConditionsShouldBeMet() const noexcept {
        return _allTheConditionsShouldBeMet;
    }

    void SetFlagMaximized(bool maximized) noexcept {
        _isMaximized = maximized;
    }
    bool GetFlagMaximized() const noexcept {
        return _isMaximized;
    }

    void SetContactsListHeaderState(const QByteArray & contactsHeaderState) {
        _contactsHeaderState = contactsHeaderState;
    }
    QByteArray GetContactsListHeaderState() const {
        return _contactsHeaderState;
    }

    void SetFiltersListHeaderState(const QByteArray & filtersHeaderState) {
        _filtersHeaderState = filtersHeaderState;
    }
    QByteArray GetFiltersListHeaderState() const {
        return _filtersHeaderState;
    }

    void SetPropertiesListHeaderState(const QByteArray & propertiesHeaderState) {
        _propertiesHeaderState = propertiesHeaderState;
    }
    QByteArray GetPropertiesListHeaderState() const {
        return _propertiesHeaderState;
    }

private:
    bool _allTheConditionsShouldBeMet = true;
    bool _isMaximized = false;
    QRect _geometry;
    QList<int> _contactSplitterState;
    QList<int> _mainWndSplitterState;
    QStringList _prevSearchAttrValues;
    QByteArray _contactsHeaderState;
    QByteArray _filtersHeaderState;
    QByteArray _propertiesHeaderState;
};

