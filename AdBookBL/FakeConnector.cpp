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
#include "Error.h"
#include "FakeConnector.h"
#include "FakeDataSource.h"
#include "ConnectionParams.h"

namespace adbook
{

FakeConnector::~FakeConnector()
{
}

void FakeConnector::Connect(const ConnectionParams & /*connectionParams*/)
{
    _connected = true;
}

void FakeConnector::Connect(const ConnectionParams & /*connectionSettings*/, const std::wstring & distinguishedName)
{
    _connected = false;
    auto & dataSource = FakeDataSource::GetDataSource();
    if (!dataSource.CheckPersonExistance(distinguishedName)) {
        throw HrError(E_INVALIDARG, L"distinguishedName", __FUNCTIONW__);
    }
    _dn = distinguishedName;
    _connected = true;
}

void FakeConnector::Disconnect()
{
    _connected = false;
    _dn.clear();
}

bool FakeConnector::IsConnected() const
{
    return _connected;
}


IDirectoryObjectPtr FakeConnector::GetDirectoryObject() const
{
    IDirectoryObjectPtr ret;
    // the implementation is not needed
    return ret;
}

void FakeConnector::Rename(const std::wstring & newName)
{
    if (!_connected) {
        HR_ERROR(E_FAIL);
    }
    auto & dataSource = FakeDataSource::GetDataSource();
    auto & adp = dataSource.GetPersonByDn(_dn);
    auto dn = adp.GetDn();
    size_t commaPos = dn.find(L',');
    size_t equalSignPos = dn.find(L'=');
    BOOST_ASSERT(commaPos != std::wstring::npos);
    BOOST_ASSERT(equalSignPos != std::wstring::npos);
    dn.replace(equalSignPos, commaPos - equalSignPos, newName.c_str());
    adp.SetStringAttr(AdAttrDn, dn);
}

void FakeConnector::UploadStringAttr(const std::wstring & attrName, const std::wstring & attrVal)
{
    if (!_connected) {
        HR_ERROR(E_FAIL);
    }
    auto & dataSource = FakeDataSource::GetDataSource();
    dataSource.GetPersonByDn(_dn).SetStringAttr(attrName, attrVal);
}

std::wstring FakeConnector::DownloadStringAttr(const std::wstring & attrName)
{
    if (!_connected) {
        HR_ERROR(E_FAIL);
    }
    auto & dataSource = FakeDataSource::GetDataSource();
    return dataSource.GetPersonByDn(_dn).GetStringAttr(attrName);
}

void FakeConnector::UploadBinaryAttr(const std::wstring & attrName, const BinaryAttrVal & bav)
{
    if (!_connected) {
        HR_ERROR(E_FAIL);
    }
    auto & dataSource = FakeDataSource::GetDataSource();
    dataSource.GetPersonByDn(_dn).SetBinaryAttr(attrName, bav);
}

BinaryAttrVal FakeConnector::DownloadBinaryAttr(const std::wstring & attrName)
{
    if (!_connected) {
        HR_ERROR(E_FAIL);
    }
    auto & dataSource = FakeDataSource::GetDataSource();
    return dataSource.GetPersonByDn(_dn).GetBinaryAttr(attrName);
}

std::wstring FakeConnector::GetLdapPath()
{
    // the implementation is not needed
    return {};
}

std::wstring FakeConnector::GetRDN()
{
    // the implementation is not needed
    return {};
}

}   // namespace adbook

