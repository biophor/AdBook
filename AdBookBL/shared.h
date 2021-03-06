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

#include "AdBookBLExport.h"

namespace adbook
{

typedef std::vector<wchar_t> WcharBuf;  // Several ADSI functions require non-const IN arguments.

WcharBuf ADBOOKBL_API ToWcharBuf (
    const std::wstring & s
);

std::wstring ADBOOKBL_API ToWstring (
    const CComBSTR & bstr
);

std::wstring ADBOOKBL_API ToWstring (
    const char * s
);

std::wstring ADBOOKBL_API LoadString (
    UINT resId
);

std::wstring ADBOOKBL_API ExtractDirFromFilePath (
    const std::wstring & filePath
);

std::wstring ADBOOKBL_API Trim(
    const std::wstring &
);

std::wstring ADBOOKBL_API ReplaceAll (
    const std::wstring & sourceString,
    const std::wstring & whatToReplace,
    const std::wstring & replacement
);

void ADBOOKBL_API ReplaceAllInPlace (
    std::wstring & sourceString,
    const std::wstring & whatToReplace,
    const std::wstring & replacement
);

std::wstring ADBOOKBL_API ToLower (
    const std::wstring & s
);

class ComAutoInitializer final {
public:
    ComAutoInitializer() {
        HRESULT hr = CoInitialize(nullptr);
        if (FAILED(hr)) {
            throw std::exception("CoInitialize() failed");
        }
    }
    ~ComAutoInitializer() {
        CoUninitialize();
    }
    ComAutoInitializer(const ComAutoInitializer&) = delete;
    ComAutoInitializer(ComAutoInitializer&&) = delete;

    ComAutoInitializer& operator=(const ComAutoInitializer&) = delete;
    ComAutoInitializer& operator=(ComAutoInitializer&&) = delete;
};

}   // namespace adbook
