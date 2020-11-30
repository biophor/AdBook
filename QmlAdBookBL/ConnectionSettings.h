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
#include "Password.h"
#include "qmladbookbl_global.h"

namespace qmladbook
{
    class QMLADBOOKBL_EXPORT ConnectionSettings final
    {
    public:
        void SetAddress(QString domain);
        QString GetAddress();
        void ConnectCurrentDomain(bool value);
        bool ConnectCurrentDomain() const;

        void ConnectAsCurrentUser(bool value);
        bool ConnectAsCurrentUser() const;

        void SetLogin(QString login);
        QString GetLogin() const;

        void SetPassword(const Password & password);
        Password GetPassword() const;

        operator adbook::ConnectionParams() const;

        friend bool operator == (const ConnectionSettings & cs1, const ConnectionSettings & cs2) {
            return cs1._login == cs2._login
                && cs1._address == cs2._address
                && cs1._connectAsCurrentUser == cs2._connectAsCurrentUser
                && cs1._connectCurrentDomain == cs2._connectCurrentDomain
                && cs1._password == cs2._password;
        }

    private:
        bool _connectCurrentDomain{ true }, _connectAsCurrentUser{ true };
        QString _address, _login;
        Password _password;
    };
}

