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
#include "MainWndSettings.h"
#include "FilterListSettings.h"

class AppSettings
{
public:
    MainWndSettings GetMainWndSettings() const {
        return _mainWndSettings;
    }
    void SetMainWndSettings(const MainWndSettings & mainWndSettings) {
        _mainWndSettings = mainWndSettings;
    }

    adbook::ConnectionParams GetConnectionParams() const {
        return _connectionParams;
    }
    void SetConnectionParams(const adbook::ConnectionParams & params) {
        _connectionParams = params;
    }

    QRect GetSettingsDlgGeometry() const {
        return _settingsDlgPosition;
    }
    void SetSettingsDlgGeometry(QRect position) {
        _settingsDlgPosition = position;
    }

    void SetFilterListSettings(const FilterListSettings & fls) {
        _filterListSettings = fls;
    }

    FilterListSettings GetFilterListSettings() const {
        return _filterListSettings;
    }
private:
    FilterListSettings _filterListSettings;
    MainWndSettings _mainWndSettings;
    QRect _settingsDlgPosition;
    adbook::ConnectionParams _connectionParams;
};

