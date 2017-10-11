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

namespace adbook
{

typedef std::vector<wchar_t> WcharBuf;  // Several ADSI functions require non-const IN arguments.

WcharBuf ADBOOKBL_API ToWcharBuf(const std::wstring & s);

std::wstring ADBOOKBL_API LoadString(UINT resId);

std::wstring ADBOOKBL_API ExtractDirFromFilePath(const std::wstring & filePath);

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
