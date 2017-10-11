// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
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

#include "stdafx.h"
#include "error.h"
#include "shared.h"
#include "dllmain.h"
#include "resource.h"

namespace adbook
{

WcharBuf ToWcharBuf(const std::wstring & s)
{
    std::vector<wchar_t> buf(s.begin(), s.end());
    buf.push_back(0);   // trailing zero
    return buf;
}

std::wstring LoadString(UINT resId)
{
    wchar_t appName[256];
    if (!LoadStringW(moduleHandle, resId, appName, _countof(appName)))
    {
        throw HrError(HRESULT_FROM_WIN32(ERROR_RESOURCE_NOT_FOUND), __FUNCTIONW__);
    }
    return appName;
}

std::wstring ExtractDirFromFilePath(const std::wstring & filePath)
{
    if (filePath.empty())
    {
        throw HrError(E_INVALIDARG, __FUNCTIONW__);
    }
    std::vector<wchar_t> buf(filePath.cbegin(), filePath.cend());
    buf.push_back(L'\0');
    LPWSTR namePtr = PathFindFileName(&buf[0]);
    if (namePtr == &buf[0] || !namePtr)
    {
        return std::wstring();
    }
    *namePtr = L'\0';
    return std::wstring(&buf[0]);
}

}   // namespace adbook
