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
#include "ConnectionParams.h"

namespace adbookcli
{

adbook::ConnectionParams ConnectionParams::ToUnderlyingType()
{
    adbook::ConnectionParams cp;
    cp.Set_ConnectDomainYouLoggedIn(_connectDomainYouLoggedIn);
    cp.Set_ConnectAsCurrentUser(_useCurrentUserCredentials);
    cp.SetAddress(StringToStdWstring(_address));
    cp.SetLogin(StringToStdWstring(_login));
    cp.SetPassword(SecureStringToStdWstring(_password));
    return cp;
}

}   // namespace adbookcli
