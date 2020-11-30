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
#include "ASettingsKeeper.h"
#include "qmladbookbl_global.h"

namespace qmladbook
{
    class QMLADBOOKBL_EXPORT SettingsKeeper : public ASettingsKeeper
    {
    public:
        SettingsKeeper() = default;
        SettingsKeeper(const SettingsKeeper &) = delete;
        SettingsKeeper(SettingsKeeper &&) = delete;
        SettingsKeeper & operator = (const SettingsKeeper &) = delete;
        SettingsKeeper & operator = (SettingsKeeper &&) = delete;
        virtual ~SettingsKeeper() = default;

        void Load(const QString & organizationName, const QString & applicationName) override;
        void Save(const QString & organizationName, const QString & applicationName) override;

        void SetConnectionSettings(const ConnectionSettings & settings) override;
        ConnectionSettings GetConnectionSettings() const override;

        void SetSearchSettings(const SearchSettings & settings) override;
        SearchSettings GetSearchSettings() const override;

    private:
        void SaveConnectionSettings(const QString & organizationName, const QString & applicationName);
        void LoadConnectionSettings(const QString & organizationName, const QString & applicationName);

        void SaveSearchSettings(const QString & organizationName, const QString & applicationName);
        void LoadSearchSettings(const QString & organizationName, const QString & applicationName);

    private:
        ConnectionSettings _connectionSettings;
        SearchSettings _searchSettings;
    };
}
