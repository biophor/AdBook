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
#include "Stdafx.h"
#include "InteropHelpers.h"

namespace adbookcli
{

using System::Runtime::InteropServices::Marshal;
using System::IntPtr;

std::wstring StringToStdWstring(String ^ s)
{
    std::wstring result;
    if (!String::IsNullOrEmpty(s)) {
        IntPtr sPtr = Marshal::StringToBSTR(s);
        try {
            if (sPtr.ToPointer() != nullptr) {
                BSTR bstrS = static_cast<BSTR>(sPtr.ToPointer());
                result = bstrS;
            }
        }
        finally {
            Marshal::FreeBSTR(sPtr);
        }
    }
    return result;
}

std::wstring SecureStringToStdWstring(SecureString ^ s)
{
    std::wstring result;
    if ((s != nullptr) && (s->Length > 0)) {
        IntPtr sPtr = Marshal::SecureStringToBSTR(s);
        try {
            if (sPtr.ToPointer() != nullptr) {
                BSTR bstrS = static_cast<BSTR>(sPtr.ToPointer());
                result = bstrS;
            }
        }
        finally {
            Marshal::ZeroFreeBSTR(sPtr);
        }
    }
    return result;
}

std::vector<BYTE> CliArrayToStdVector(cli::array<Byte> ^ data)
{
    std::vector<BYTE> ret(data->Length);
    if (data->Length > 0) {
        IntPtr p(&ret[0]);
        Marshal::Copy(data, 0, p, data->Length);
    }
    return ret;
}

cli::array<Byte> ^ StdVectorToCliArray(std::vector<BYTE> data)
{
    cli::array<Byte> ^ ret = gcnew cli::array<Byte>(static_cast<System::Int32>(data.size()));
    if (data.size() > 0) {
        IntPtr p(&data[0]);
        Marshal::Copy(p, ret, 0, ret->Length);
    }
    return ret;
}

void ListCtrlHelper::EnableTooltipsForListCtrl(System::IntPtr listCtrlHandle)
{
    ListView_SetExtendedListViewStyleEx(static_cast<HWND>(static_cast<HANDLE>(listCtrlHandle)),
        LVS_EX_LABELTIP, LVS_EX_LABELTIP);
}

}   // namespace adbookcli

