// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
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

#include "stdafx.h"
#include "shared.h"
#include "error.h"
#include "ConnectionParams.h"

namespace adbook
{
std::wstring ConnectionParams::GetLogin() const
{
    return _login;
}

std::wstring ConnectionParams::GetPassword() const {
    return _password;
}

bool ConnectionParams::Get_ConnectAsCurrentUser() const {
    return _connectAsCurrentUser;
}

void ConnectionParams::Set_ConnectAsCurrentUser(bool connectAsCurrentUser) {
    _connectAsCurrentUser = connectAsCurrentUser;
}

bool ConnectionParams::Get_ConnectDomainYouLoggedIn() const {
    return _connectCurrentDomain;
}

void ConnectionParams::Set_ConnectDomainYouLoggedIn(bool connectDomainYouLoggedIn) {
    _connectCurrentDomain = connectDomainYouLoggedIn;
}

void ConnectionParams::SetAddress(const std::wstring & address) {
    _address = Trim(address);
}

std::wstring ConnectionParams::GetAddress() const {
    return _address;
}

void ConnectionParams::SetLogin(const std::wstring & login) {
    _login = Trim(login);
}

void ConnectionParams::SetPassword(const std::wstring & password) {
    _password = Trim(password);
}

bool ConnectionParams::IsConsistent() const noexcept
{
    if (!_connectAsCurrentUser && (_login.empty() || _password.empty())) {
        return false;
    }
    if (!_connectCurrentDomain && _address.empty()) {
        return false;
    }
    return true;
}

} // namespace adbook
