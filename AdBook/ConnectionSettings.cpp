/*
Copyright (C) 2015 Goncharov Andrei.

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
const wchar_t connectionSettingsSection[] = L"Connection";
const wchar_t dcNameParam[] = L"dc";
const wchar_t userNameParam[] = L"userName";
const wchar_t passwordParam[] = L"password";
const wchar_t currentUserCredParam[] = L"currentUserCred";
const wchar_t currentDomainParam[] = L"currentDomain";
const wchar_t * displayPasswordParam = L"displayPassword";
const wchar_t * forgetPasswordParam = L"forgetPassword";
}

void ConnectionSettings::ForgetPassword(const bool forget) noexcept
{
    forgetPassword_ = forget;
}

bool ConnectionSettings::ForgetPassword() const noexcept
{
    return forgetPassword_;
}

void ConnectionSettings::DisplayPassword(const bool display) noexcept
{
    displayPassword_ = display;
}

bool ConnectionSettings::DisplayPassword() const noexcept
{
    return displayPassword_;
}

void ConnectionSettings::Save()
{
    CWinApp * app = AfxGetApp();
    // connection settings
    VERIFY(app);
    VERIFY(app->WriteProfileStringW(connectionSettingsSection, dcNameParam, GetDC().c_str()));
    VERIFY(app->WriteProfileStringW(connectionSettingsSection, userNameParam, GetLogin().c_str()));
    if (ForgetPassword())
    {
        VERIFY(app->WriteProfileStringW(connectionSettingsSection, passwordParam, L""));
    }
    else
    {
        VERIFY(app->WriteProfileStringW(connectionSettingsSection, passwordParam, GetPassword().c_str()));
    }
    
    VERIFY(app->WriteProfileInt(connectionSettingsSection, currentUserCredParam, CurrentUserCredentials() ? 1 : 0));
    VERIFY(app->WriteProfileInt(connectionSettingsSection, currentDomainParam, CurrentDomain() ? 1 : 0));
    VERIFY(app->WriteProfileInt(connectionSettingsSection, displayPasswordParam, DisplayPassword()));
    VERIFY(app->WriteProfileInt(connectionSettingsSection, forgetPasswordParam, ForgetPassword()));    
}

void ConnectionSettings::Load()
{
    CWinApp * app = AfxGetApp();
    VERIFY(app);
    auto getcs = [app](const wchar_t* section, const wchar_t * param) -> CString {
        return app->GetProfileString(section, param);
    };
    auto getbool = [app](const wchar_t* section, const wchar_t * param, bool def) {
        return (!!app->GetProfileInt(section, param, static_cast<int>(def)));
    };
    // connection settings
    SetDC(getcs(connectionSettingsSection, dcNameParam));
    SetLogin(getcs(connectionSettingsSection, userNameParam));
    SetPassword(getcs(connectionSettingsSection, passwordParam));
    CurrentUserCredentials(getbool(connectionSettingsSection, currentUserCredParam, true));
    CurrentDomain(getbool(connectionSettingsSection, currentDomainParam, true));
    DisplayPassword(getbool(connectionSettingsSection, displayPasswordParam, false));
    ForgetPassword(getbool(connectionSettingsSection, forgetPasswordParam, true));    
}

