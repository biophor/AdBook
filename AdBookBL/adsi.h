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

#pragma once

#include "AdBookBLExport.h"


namespace adbook
{

typedef CComQIPtr<IADs, &IID_IADs>                          IADsPtr;
typedef CComQIPtr<IADsUser, &IID_IADsUser>                  IADsUserPtr;
typedef CComQIPtr<IADsPathname, &IID_IADsPathname>          IADsPathNamePtr;
typedef CComQIPtr<IADsContainer, &IID_IADsContainer>        IADsContainerPtr;
typedef CComQIPtr<IDirectoryObject, &IID_IDirectoryObject>  IDirectoryObjectPtr;
typedef CComQIPtr<IDirectorySearch, &IID_IDirectorySearch>  IDirectorySearchPtr;
typedef CComQIPtr<IDispatch, &IID_IDispatch>                MyIDispatchPtr;

const wchar_t * const AdAttrDn = L"distinguishedName";
const wchar_t * const AdAttrWritableAttrs = L"allowedAttributesEffective";
const wchar_t * const AdAttrPhoto = L"thumbnailPhoto";

using LdapAttrName = std::wstring;
using StringAttrVal = std::wstring;
using BinaryAttrVal = std::vector<BYTE>;


}   // namespace adbook
