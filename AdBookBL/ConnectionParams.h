/*
Copyright (C) 2015-2020 Goncharov Andrei.

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
#include "AdBookBLExport.h"

namespace adbook
{

class ADBOOKBL_API ConnectionParams final
{
public:
    template <class T>
    void SetDomainController(T && dc);

    template <class T>
    void SetLdapPath(T && ldapPath);

    std::wstring GetDomainController() const;

    std::wstring GetLdapPath() const;

    template <class T>
    void SetLogin(T && login);

    template <class T>
    void SetPassword(T && password);

    void UseCurrentUserCredentials(bool useCurrentUserCredentials) noexcept;
    bool IsCurrentUserCredentialsUsed() const noexcept;
    void ConnectDomainYouAreLoggedIn(bool connectDomainYouAreLoggedIn) noexcept;
    bool CurrentDomain() const noexcept;
    std::wstring GetLogin() const;
    std::wstring GetPassword() const;

private:
    // address_ can be one of the following:
    // * active directory domain name
    // * active directory domain controller address (ip-address, dns name, netbios name)
    // * LDAP full path. In this case _address should start with 'LDAP://' and point to
    //   the specifiec object in AD LDS instance
    std::wstring address_;
    bool useCurrentUserCred_ = true;
    bool connectCurrentDomain_ = true;
    std::wstring login_, _password;
};

template <class T>
void ConnectionParams::SetDomainController(T && dc)
{
    address_ = std::forward<T>(dc);
}

template <class T>
void ConnectionParams::SetLdapPath(T && ldapPath)
{
    address_ = std::forward<T>(ldapPath);
}

template <class T>
void ConnectionParams::SetLogin(T && login)
{
    login_ = std::forward<T>(login);
}

template <class T>
void ConnectionParams::SetPassword(T && password)
{
    _password = std::forward<T>(password);
}


}

