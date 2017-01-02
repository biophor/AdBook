/*
Copyright (C) 2015-2017 Goncharov Andrei.

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
#include "ConnectionSettings.h"

namespace
{
const char connectionSettingsSection[] = "Connection";
const char dcNameParam[] = "dc";
const char userNameParam[] = "userName";
const char passwordParam[] = "password";
const char currentUserCredParam[] = "currentUserCred";
const char currentDomainParam[] = "currentDomain";
}

ConnectionSettings & ConnectionSettings::Instance() {
    static ConnectionSettings cs;
    return cs;
}

void ConnectionSettings::ForgetPassword(const bool forget) noexcept
{
    forgetPassword_ = forget;
}

bool ConnectionSettings::ForgetPassword() const noexcept
{
    return forgetPassword_;
}

void ConnectionSettings::Save()
{
    QSettings settings(QApplication::organizationName(), QApplication::applicationName());
    settings.beginGroup(connectionSettingsSection);
    settings.setValue(dcNameParam, QString::fromStdWString(GetDC()));
    settings.setValue(userNameParam, QString::fromStdWString(GetLogin()));
    if (ForgetPassword()) {
        settings.setValue(passwordParam, QStringLiteral(""));
    }
    else {
        settings.setValue(passwordParam, QString::fromStdWString(GetPassword()));
    }
    settings.setValue(currentUserCredParam, CurrentUserCredentials());
    settings.setValue(currentDomainParam, CurrentDomain());
#pragma warning(push, 3)
#pragma warning(disable: 4003)
    BOOST_SCOPE_EXIT(&settings) { settings.endGroup(); }BOOST_SCOPE_EXIT_END
#pragma warning(pop)    
}

void ConnectionSettings::Load()
{
    QSettings settings(QApplication::organizationName(), QApplication::applicationName());
    bool settingsWasPreviouslySaved = settings.contains(QString(connectionSettingsSection) + "/" + dcNameParam);
    if (!settingsWasPreviouslySaved) {
        QString appNameInMfcVersion = "Active Directory Contact Book";
        QSettings oldSettings(QSettings::NativeFormat, QSettings::UserScope, 
                              appNameInMfcVersion, appNameInMfcVersion);
        Load(oldSettings);
    }
    else {
        Load(settings);
    }
}

void ConnectionSettings::Load(QSettings & settings)
{
    settings.beginGroup(connectionSettingsSection);
    SetDC(settings.value(dcNameParam).toString().toStdWString());
    SetLogin(settings.value(userNameParam).toString().toStdWString());
    SetPassword(settings.value(passwordParam).toString().toStdWString());
    CurrentUserCredentials(settings.value(currentUserCredParam, true).toBool());
    CurrentDomain(settings.value(currentDomainParam, true).toBool());
#pragma warning(push, 3)
#pragma warning(disable: 4003)
    BOOST_SCOPE_EXIT(&settings) { settings.endGroup(); } BOOST_SCOPE_EXIT_END
#pragma warning(pop)
}

