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
#include "adsi.h"

namespace adbook {

    void ADsGetObjectWrp (
        const std::wstring & ldapPath,
        const IID & iid,
        void ** pp
    )
    {
        const HRESULT hr = ADsGetObject (
            ldapPath.c_str(),
            iid,
            pp
        );
        if (FAILED(hr)) {
            throw HrError(hr, L"ADsGetObject() failed", __FUNCTIONW__);
        }
    }

    void ADsOpenObjectWrp (
        const std::wstring & ldapPath,
        const std::wstring & login,
        const std::wstring & password,
        const IID & iid,
        void ** pp
    )
    {
        const HRESULT hr = ADsOpenObject (
            ldapPath.c_str(),
            login.c_str(),
            password.c_str(),
            ADS_SECURE_AUTHENTICATION,
            iid,
            pp
        );
        if (FAILED(hr)) {
            throw HrError(hr, L"ADsOpenObject() failed", __FUNCTIONW__);
        }
    }

    IDirectoryObjectPtr ADsGetDirectoryObject (
        const std::wstring & ldapPath
    )
    {
        if (ldapPath.empty()) {
            throw HrError(E_INVALIDARG, L"ldapPath is empty.", __FUNCTIONW__);
        }
        IDirectoryObjectPtr objectPtr;

        ADsGetObjectWrp (
            ldapPath,
            IID_IDirectoryObject,
            reinterpret_cast<void**>(&objectPtr.p)
        );
        return objectPtr;
    }

    IDirectoryObjectPtr ADsOpenDirectoryObject (
        const std::wstring & ldapPath,
        const std::wstring & login,
        const std::wstring & password
    )
    {
        if (ldapPath.empty()) {
            throw HrError(E_INVALIDARG, L"ldapPath is empty.", __FUNCTIONW__);
        }
        if (login.empty()) {
            throw HrError(E_INVALIDARG, L"login is empty.", __FUNCTIONW__);
        }
        if (password.empty()) {
            throw HrError(E_INVALIDARG, L"password is empty.", __FUNCTIONW__);
        }
        IDirectoryObjectPtr objectPtr;

        ADsOpenObjectWrp (
            ldapPath,
            login,
            password,
            IID_IDirectoryObject,
            reinterpret_cast<void **>(&objectPtr.p)
        );
        return objectPtr;
    }
}