// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
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

QString ProtectPassword(const QString & s)
{
    if (s.isEmpty()) {
        return QString();
    }    
    auto w = s.toStdWString();
    std::vector<wchar_t> bufToProtect(w.cbegin(), w.cend());
    DATA_BLOB db;
    db.cbData = static_cast<DWORD>(bufToProtect.size() * sizeof(wchar_t));
    db.pbData = (BYTE*)(&bufToProtect[0]);
    DATA_BLOB rdb;
    if (!CryptProtectData(&db, nullptr, nullptr, nullptr, nullptr, 0, &rdb)) {
        throw adbook::HrError(HRESULT_FROM_WIN32(GetLastError()), 
            L"CryptProtectData() failed.", __FUNCTIONW__
        );
    }
    QByteArray rba((const char*)rdb.pbData, rdb.cbData);
    LocalFree(rdb.pbData);
    return rba.toBase64();
}

QString UnprotectPassword(const QString & s)
{
    if (s.isEmpty()) {
        return QString();
    }
    QByteArray ba64 = QByteArray::fromStdString(s.toStdString());
    QByteArray ba = QByteArray::fromBase64(ba64);    
    DATA_BLOB db;
    db.cbData = static_cast<DWORD>(ba.size());
    db.pbData = (BYTE*)(ba.data());
    DATA_BLOB rdb;
    if (!CryptUnprotectData(&db, nullptr, nullptr, nullptr, nullptr, 0, &rdb)) {
        throw adbook::HrError(HRESULT_FROM_WIN32(GetLastError()),
            L"CryptUnprotectData() failed.", __FUNCTIONW__
        );
    }    
    QString ret = QString::fromWCharArray((const wchar_t*)rdb.pbData, rdb.cbData/sizeof(wchar_t));
    LocalFree(rdb.pbData);
    return ret;
}

ConnectionSettings & ConnectionSettings::Instance() {
    static ConnectionSettings cs;
    return cs;
}

void ConnectionSettings::Save()
{
    QSettings settings(QApplication::organizationName(), QApplication::applicationName());
    settings.beginGroup(connectionSettingsSection);
    settings.setValue(dcNameParam, QString::fromStdWString(GetDC()));
    settings.setValue(userNameParam, QString::fromStdWString(GetLogin()));    
    QString protectedPassword = ProtectPassword(QString::fromStdWString(GetPassword()));
    settings.setValue(passwordParam, protectedPassword);    
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
        Load(oldSettings, true);
    }
    else {
        Load(settings, false);
    }
}

void ConnectionSettings::Load(QSettings & settings, bool dontDecryptPassword)
{
    settings.beginGroup(connectionSettingsSection);
    SetDC(settings.value(dcNameParam).toString().toStdWString());
    SetLogin(settings.value(userNameParam).toString().toStdWString());
    QString protectedPassword = settings.value(passwordParam).toString();    
    CurrentUserCredentials(settings.value(currentUserCredParam, true).toBool());
    CurrentDomain(settings.value(currentDomainParam, true).toBool());
    QString unprotectedPassword = dontDecryptPassword ? protectedPassword : 
        UnprotectPassword(protectedPassword);
    SetPassword(unprotectedPassword.toStdWString());
#pragma warning(push, 3)
#pragma warning(disable: 4003)
    BOOST_SCOPE_EXIT(&settings) { settings.endGroup(); } BOOST_SCOPE_EXIT_END
#pragma warning(pop)
}

