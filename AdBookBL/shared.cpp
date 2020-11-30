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
#include "error.h"
#include "shared.h"
#include "dllmain.h"
#include "resource.h"

namespace adbook
{

WcharBuf ToWcharBuf( const std::wstring & s )
{
    std::vector<wchar_t> buf(s.begin(), s.end());
    buf.push_back(0);   // trailing zero
    return buf;
}

std::wstring ADBOOKBL_API ToWstring (
    const CComBSTR & bstr
)
{
    if (bstr.Length() == 0) {
        return {};
    }
    return std::wstring(static_cast<LPCWSTR>(bstr), bstr.Length());
}

std::wstring ToWstring( const char * s )
{
    if (nullptr == s) {
        return {};
    }

    const size_t slen = strlen(s);
    if (0 == slen) {
        return {};
    }

    constexpr size_t stackBufSize = 256;
    size_t numCharsConverted{};

    if (slen < stackBufSize) {
        wchar_t wBuf[stackBufSize]{};
        mbstowcs_s(&numCharsConverted, wBuf, s, _TRUNCATE);
        return wBuf;
    }
    WcharBuf buf(slen + 1, 0);
    mbstowcs_s(&numCharsConverted, buf.data(), buf.size(), s, _TRUNCATE);
    return buf.data();
}

std::wstring LoadString( UINT resId )
{
    wchar_t appName[256];
    if (!LoadStringW(moduleHandle, resId, appName, _countof(appName)))
    {
        throw HrError(HRESULT_FROM_WIN32(ERROR_RESOURCE_NOT_FOUND), __FUNCTIONW__);
    }
    return appName;
}

std::wstring ExtractDirFromFilePath( const std::wstring & filePath )
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

std::wstring Trim( const std::wstring & s )
{
    if (s.empty()) {
        return {};
    }
    const wchar_t spaces[] = L" \t\n\r";
    size_t startIndex = s.find_first_not_of(spaces);
    if (std::wstring::npos == startIndex) {
        return {};
    }
    size_t endIndex = s.find_last_not_of(spaces);
    std::wstring ret(s.substr(startIndex, endIndex - startIndex + 1));
    return ret;
}

std::wstring ReplaceAll (
    const std::wstring & sourceString,
    const std::wstring & whatToReplace,
    const std::wstring & replacement
)
{
    if (sourceString.empty()) {
        return sourceString;
    }
    if (whatToReplace == replacement) {
        return sourceString;
    }
    if (whatToReplace.empty()) {
        throw HrError(E_INVALIDARG, L"whatToReplace is empty.", __FUNCTIONW__);
    }
    std::wstring ret = sourceString;
    size_t pos = ret.find(whatToReplace);
    const size_t whatToReplaceSize = whatToReplace.size();
    const size_t replacementSize = replacement.size();
    while (pos != std::wstring::npos)
    {
        ret.replace(pos, whatToReplaceSize, replacement);
        pos = ret.find(whatToReplace, pos + replacementSize);
    }
    return ret;
}

void ADBOOKBL_API ReplaceAllInPlace (
    std::wstring & sourceString,
    const std::wstring & whatToReplace,
    const std::wstring & replacement
)
{
    if (sourceString.empty()) {
        return;
    }
    if (whatToReplace == replacement) {
        return;
    }
    if (whatToReplace.empty()) {
        throw HrError(E_INVALIDARG, L"whatToReplace is empty.", __FUNCTIONW__);
    }
    size_t pos = sourceString.find(whatToReplace);
    const size_t whatToReplaceSize = whatToReplace.size();
    const size_t replacementSize = replacement.size();
    while (pos != std::wstring::npos)
    {
        sourceString.replace(pos, whatToReplaceSize, replacement);
        pos = sourceString.find(whatToReplace, pos + replacementSize);
    }
}

std::wstring ToLower( const std::wstring & s )
{
    if (s.empty()) {
        return s;
    }
    WcharBuf tmp = ToWcharBuf(s);
    CharLowerBuffW(tmp.data(), static_cast<DWORD>(tmp.size()));
    return tmp.data();
}


}   // namespace adbook
