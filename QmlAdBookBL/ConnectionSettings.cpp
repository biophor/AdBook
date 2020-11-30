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
#include "ConnectionSettings.h"

namespace qmladbook
{
    void ConnectionSettings::SetAddress(QString domain) {
        _address = domain;
    }

    QString ConnectionSettings::GetAddress() {
        return _address;
    }

    void ConnectionSettings::ConnectCurrentDomain(bool value) {
        _connectCurrentDomain = value;
    }

    bool ConnectionSettings::ConnectCurrentDomain() const {
        return _connectCurrentDomain;
    }

    void ConnectionSettings::SetLogin(QString login) {
        _login = login;
    }

    QString ConnectionSettings::GetLogin() const {
        return _login;
    }

    void ConnectionSettings::SetPassword(const Password & password) {
        _password = password;
    }

    Password ConnectionSettings::GetPassword() const {
        return _password;
    }

    void ConnectionSettings::ConnectAsCurrentUser(bool value) {
        _connectAsCurrentUser = value;
    }

    bool ConnectionSettings::ConnectAsCurrentUser() const {
        return _connectAsCurrentUser;
    }

    ConnectionSettings::operator adbook::ConnectionParams() const
    {
        adbook::ConnectionParams params;

        params.Set_ConnectDomainYouLoggedIn(_connectCurrentDomain);
        if (!_connectCurrentDomain) {
            params.SetAddress(_address.toStdWString());
        }

        params.Set_ConnectAsCurrentUser(_connectAsCurrentUser);
        if (!_connectAsCurrentUser)
        {
            params.SetLogin(_login.toStdWString());
            QString strPassword;
            _password.TryToGetAsPlainTextString(strPassword);
            params.SetPassword(strPassword.toStdWString());
        }
        return params;
    }
}
