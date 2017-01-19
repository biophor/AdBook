// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
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
const wchar_t protectedPasswordParam[] = L"protectedPassword";
const wchar_t currentUserCredParam[] = L"currentUserCred";
const wchar_t currentDomainParam[] = L"currentDomain";
const wchar_t * displayPasswordParam = L"displayPassword";
const wchar_t * forgetPasswordParam = L"forgetPassword";
}

std::vector<BYTE> ProtectPassword(const std::wstring & password)
{
    if (password.empty())
    {
        return std::vector<BYTE>();
    }
    std::vector<wchar_t> buffToProtect(password.cbegin(), password.cend());
    DATA_BLOB db;
    db.cbData = boost::numeric_cast<DWORD>(password.length() * sizeof(wchar_t));
    db.pbData = reinterpret_cast<BYTE*>(&buffToProtect[0]);
    DATA_BLOB dbr;
    if (!CryptProtectData(&db, nullptr, nullptr, nullptr, nullptr, 0, &dbr))
    {
        HR_ERROR(HRESULT_FROM_WIN32(GetLastError()));
    }
    std::vector<BYTE> result(dbr.pbData, dbr.pbData + boost::numeric_cast<size_t>(dbr.cbData));
    LocalFree(dbr.pbData);
    return result;
}

std::wstring UnprotectPassword(std::vector<BYTE> protectedPassword)
{
    if (protectedPassword.empty())
    {
        return std::wstring();
    }    
    DATA_BLOB db;
    db.cbData = boost::numeric_cast<DWORD>(protectedPassword.size());
    db.pbData = &protectedPassword[0];
    DATA_BLOB dbr;
    if (!CryptUnprotectData(&db, nullptr, nullptr, nullptr, nullptr, 0, &dbr))
    {
        HR_ERROR(HRESULT_FROM_WIN32(GetLastError()));
    }
    return std::wstring(
        reinterpret_cast<const wchar_t *>(dbr.pbData), 
        boost::numeric_cast<size_t>(dbr.cbData)/sizeof(wchar_t)
    );
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
    VERIFY(app->WriteProfileInt(connectionSettingsSection, currentUserCredParam, CurrentUserCredentials() ? 1 : 0));
    VERIFY(app->WriteProfileInt(connectionSettingsSection, currentDomainParam, CurrentDomain() ? 1 : 0));
    VERIFY(app->WriteProfileInt(connectionSettingsSection, displayPasswordParam, DisplayPassword()));
    VERIFY(app->WriteProfileStringW(connectionSettingsSection, passwordParam, GetPassword().c_str()));
    std::wstring password = GetPassword();
    if (!password.empty())
    {    
        std::vector<BYTE> protectedPassword = ProtectPassword(password);
        VERIFY(app->WriteProfileBinary(connectionSettingsSection, protectedPasswordParam,
            &protectedPassword[0], boost::numeric_cast<UINT>(protectedPassword.size()))
        );
    }
    else
    {
        VERIFY(app->WriteProfileBinary(connectionSettingsSection, protectedPasswordParam, nullptr, 0));
    }    
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
    auto getbin = [app](const wchar_t* section, const wchar_t * param) {
        BYTE * dataPtr = nullptr;
        UINT dataSizeInBytes = 0;
        if (app->GetProfileBinary(section, param, &dataPtr, &dataSizeInBytes) && dataPtr && dataSizeInBytes) {            
            return std::vector<BYTE>(dataPtr, dataPtr + boost::numeric_cast<size_t>(dataSizeInBytes));
        }
        else {
            return std::vector<BYTE>();
        }
    };
    // connection settings
    SetDC(getcs(connectionSettingsSection, dcNameParam));
    SetLogin(getcs(connectionSettingsSection, userNameParam));    
    CurrentUserCredentials(getbool(connectionSettingsSection, currentUserCredParam, true));
    CurrentDomain(getbool(connectionSettingsSection, currentDomainParam, true));
    DisplayPassword(getbool(connectionSettingsSection, displayPasswordParam, false));
    std::vector<BYTE> protectedPassword = getbin(connectionSettingsSection, protectedPasswordParam);
    std::wstring password;
    if (!protectedPassword.empty())
    {
        password = UnprotectPassword(protectedPassword);        
    }
    SetPassword(password);
}

