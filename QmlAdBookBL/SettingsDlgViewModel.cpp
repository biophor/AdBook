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
#include "SettingsDlgViewModel.h"
#include "ADependencyResolver.h"

namespace qmladbook
{
    SettingsDlgViewModel::SettingsDlgViewModel()
        : _dependencyResolver{ GetDependencyResolver() }
    { }

    bool SettingsDlgViewModel::connectCurrentDomain() const {
        return _connectCurrentDomain;
    }

    void SettingsDlgViewModel::setConnectCurrentDomain(bool newValue)
    {
        _connectCurrentDomain = newValue;
        emit connectCurrentDomainChanged();
    }

    bool SettingsDlgViewModel::connectAsCurrentUser() const {
        return _connectAsCurrentUser;
    }

    void SettingsDlgViewModel::setConnectAsCurrentUser(bool newValue)
    {
        _connectAsCurrentUser = newValue;
        emit connectAsCurrentUserChanged();
    }

    QString SettingsDlgViewModel::address() const {
        return _address;
    }

    void SettingsDlgViewModel::setAddress(QString newValue)
    {
        _address = newValue;
        emit addressChanged();
    }

    QString SettingsDlgViewModel::login() const {
        return _login;
    }

    void SettingsDlgViewModel::setLogin(QString newValue)
    {
        _login = newValue;
        emit loginChanged();
    }

    QString SettingsDlgViewModel::password() const
    {
        QString plainTextPassword;
        _password.TryToGetAsPlainTextString(plainTextPassword);
        return plainTextPassword;
    }

    void SettingsDlgViewModel::setPassword(QString newValue)
    {
        _password.TryToSet(newValue);
        emit passwordChanged();
    }

    void SettingsDlgViewModel::save()
    {
        auto settingsKeeper = _dependencyResolver->GetSettingsKeeper();

        qmladbook::ConnectionSettings settings;
        settings.ConnectCurrentDomain(_connectCurrentDomain);
        settings.ConnectAsCurrentUser(_connectAsCurrentUser);
        settings.SetAddress(_address);
        settings.SetLogin(_login);
        settings.SetPassword(_password);

        settingsKeeper->SetConnectionSettings(settings);
        settingsKeeper->Save(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    }

    void SettingsDlgViewModel::load()
    {
        auto settingsKeeper = _dependencyResolver->GetSettingsKeeper();

        settingsKeeper->Load(QCoreApplication::organizationName(), QCoreApplication::applicationName());
        qmladbook::ConnectionSettings settings = settingsKeeper->GetConnectionSettings();
        _connectCurrentDomain = settings.ConnectCurrentDomain();
        emit connectCurrentDomainChanged();
        _connectAsCurrentUser = settings.ConnectAsCurrentUser();
        emit connectAsCurrentUserChanged();
        _address = settings.GetAddress();
        emit addressChanged();
        _login = settings.GetLogin();
        emit loginChanged();
        _password = settings.GetPassword();
        emit passwordChanged();
    }

    bool SettingsDlgViewModel::testSettings()
    {
        auto settingsChecker = _dependencyResolver->GetSettingsChecker();

        qmladbook::ConnectionSettings settings;
        settings.ConnectCurrentDomain(_connectCurrentDomain);
        settings.ConnectAsCurrentUser(_connectAsCurrentUser);
        settings.SetAddress(_address);
        settings.SetLogin(_login);
        settings.SetPassword(_password);

        qmladbook::ASettingsChecker::CheckResult checkResult = settingsChecker->Check(settings);
        _testSettingsDetails = checkResult.details;
        if (_testSettingsDetails.isEmpty()) {
            _testSettingsDetails = "";  // for qml
        }
        emit testSettingsDetailsChanged();
        if (checkResult.status == qmladbook::ASettingsChecker::CheckResultStatus::Ok) {
            return true;
        }
        return false;
    }

    QString SettingsDlgViewModel::testSettingsDetails() const {
        return _testSettingsDetails;
    }

    void SettingsDlgViewModel::SetDependencyResolver(std::shared_ptr< ADependencyResolver> resolver) {
        _dependencyResolver = resolver;
    }
}
