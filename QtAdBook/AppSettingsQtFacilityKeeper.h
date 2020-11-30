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
#include "AbstractAppSettingsKeeper.h"
#include "AppSettings.h"
#include "FilterListSettings.h"

class PasswordDecryptionError : public adbook::HrError { 
    using HrError::HrError;
};

class PasswordEncryptionError : public adbook::HrError { 
    using HrError::HrError;
};

class AppSettingsQtFacilityKeeper : public AbstractAppSettingsKeeper
{
public:
    void Load(AppSettings & appSettings) override;
    void Save(const AppSettings & appSettings) override;

private:
    void LoadFilterListSettings(FilterListSettings & filterListSettings);
    void SaveFilterListSettings(const FilterListSettings & filterListSettings);

    void LoadMainWndSettings(MainWndSettings & mainWndSettings);
    void SaveMainWndSettings(const MainWndSettings & mainWndSettings);

    void LoadConnectionParams(adbook::ConnectionParams & connectionParams);
    void SaveConnectionParams(const adbook::ConnectionParams & connectionParams);

    void LoadSettingsDlgGeometry(QRect & geom);
    void SaveSettingsDlgGeometry(const QRect & geom);
};

