/*
Copyright (C) 2015-2017 Goncharov Andrei.

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

namespace adbookcli
{
using System::String;
using System::Security::SecureString;
using System::Byte;

std::wstring StringToStdWstring(String ^ s);
std::wstring SecureStringToStdWstring(SecureString ^ s);

std::vector<BYTE> CliArrayToStdVector(cli::array<Byte> ^ data);
cli::array<Byte> ^ StdVectorToCliArray(std::vector<BYTE> data);

public ref struct ListCtrlHelper abstract sealed
{
    static void EnableTooltipsForListCtrl(System::IntPtr listCtrlHandle);
};


}   // namespace adbookcli

