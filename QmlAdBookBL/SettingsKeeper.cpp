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
#include "QSettingsAutoEndGroup.h"
#include "SettingsKeeper.h"

namespace qmladbook
{

    namespace ConnectionSettingsStrings
    {
        const char connectionSettingsSection[] = "ConnectionSettings";
        const char addressParam[] = "address";
        const char loginParam[] = "login";
        const char passwordParam[] = "password";
        const char currentAsCurrentUserParam[] = "connectAsCurrentUser";
        const char connectCurrentDomainParam[] = "connectCurrentDomain";
    }

    void SettingsKeeper::Load(const QString & organizationName, const QString & applicationName)
    {
        LoadConnectionSettings(organizationName, applicationName);
        LoadSearchSettings(organizationName, applicationName);
    }

    void SettingsKeeper::Save(const QString & organizationName, const QString & applicationName)
    {
        SaveConnectionSettings(organizationName, applicationName);
        SaveSearchSettings(organizationName, applicationName);
    }

    void SettingsKeeper::SetConnectionSettings(const ConnectionSettings & settings)
    {
        _connectionSettings = settings;
    }

    ConnectionSettings SettingsKeeper::GetConnectionSettings() const
    {
        return _connectionSettings;
    }

    void SettingsKeeper::SaveConnectionSettings(const QString & organizationName, const QString & applicationName)
    {
        using namespace ConnectionSettingsStrings;
        QSettings settings(organizationName, applicationName);
        QSettingsAutoGroup autoGroup(settings, connectionSettingsSection);

        settings.setValue(currentAsCurrentUserParam, _connectionSettings.ConnectAsCurrentUser());
        settings.setValue(connectCurrentDomainParam, _connectionSettings.ConnectCurrentDomain());

        settings.setValue(addressParam, _connectionSettings.GetAddress());
        settings.setValue(loginParam, _connectionSettings.GetLogin());
        QString password = _connectionSettings.GetPassword().Serialize();
        settings.setValue(passwordParam, password);
    }

    void SettingsKeeper::LoadConnectionSettings(const QString & organizationName, const QString & applicationName)
    {
        using namespace ConnectionSettingsStrings;
        QSettings settings(organizationName, applicationName);
        QSettingsAutoGroup autoGroup(settings, connectionSettingsSection);

        _connectionSettings.ConnectAsCurrentUser(settings.value(currentAsCurrentUserParam, true).toBool());
        _connectionSettings.ConnectCurrentDomain(settings.value(connectCurrentDomainParam, true).toBool());
        _connectionSettings.SetAddress(settings.value(addressParam).toString());
        _connectionSettings.SetLogin(settings.value(loginParam).toString());

        QString serializedPassword = settings.value(passwordParam).toString();
        Password password;
        password.Deserialize(serializedPassword);
        _connectionSettings.SetPassword(password);
    }

    namespace SearchFilterSettingsStrings
    {
        const char searchFiltersSettingsSection[] = "SearchFiltersSettings";

        const char searchFiltersEnabledParam[] = "SearchFilterEnabled";
        const char allFilteringRulesMustBeMetParam[] = "AllFilteringRulesMustBeMet";
        const char searchFiltersParam[] = "SearchFilters";
    }

    void SettingsKeeper::SaveSearchSettings (
        const QString & organizationName,
        const QString & applicationName
    )
    {
        using namespace SearchFilterSettingsStrings;
        QSettings settings(organizationName, applicationName);
        QSettingsAutoGroup autoGroup(settings, searchFiltersSettingsSection);

        settings.setValue(searchFiltersEnabledParam, _searchSettings.SearchFiltersEnabled());
        settings.setValue(allFilteringRulesMustBeMetParam, _searchSettings.AllFilteringRulesMustBeMet());
        settings.setValue(searchFiltersParam, _searchSettings.GetSerializedFilters());
    }

    void SettingsKeeper::LoadSearchSettings (
        const QString & organizationName,
        const QString & applicationName
    )
    {
        using namespace SearchFilterSettingsStrings;
        QSettings settings(organizationName, applicationName);
        QSettingsAutoGroup autoGroup(settings, searchFiltersSettingsSection);

        _searchSettings.EnableSearchFilters(
            settings.value(
                searchFiltersEnabledParam,
                SearchSettings::SearchFiltersEnabled_DefaultValue()
            ).toBool()
            );

        _searchSettings.AllFilteringRulesMustBeMet(
            settings.value(
                allFilteringRulesMustBeMetParam,
                SearchSettings::AllFilteringRulesMustBeMet_DefaultValue()
            ).toBool()
        );

        _searchSettings.SetSerializedFilters(settings.value(searchFiltersParam).toByteArray());
    }

    void SettingsKeeper::SetSearchSettings(const SearchSettings & settings)
    {
        _searchSettings = settings;
    }

    SearchSettings SettingsKeeper::GetSearchSettings() const
    {
        return _searchSettings;
    }
}

