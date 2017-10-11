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
#include "adsi.h"
#include "AdBookBLExport.h"
#include "AbstractAdConnector.h"

namespace adbook
{
// FakeConnector is used for searching if you specified the option '/FakeDataSource' for AdBook.exe
// This can be useful if you cant create an AD LDS instance for testing purposes.
class ADBOOKBL_API FakeConnector : public AbstractAdConnector
{
public:

    virtual ~FakeConnector() override;
    void Connect(const ConnectionParams & connectionParams) override;
    void Connect(const ConnectionParams & connectionSettings, const std::wstring & distinguishedName) override;
    void Disconnect() override;
    bool IsConnected() const override;

    std::wstring GetLdapPath() override;
    std::wstring GetRDN() override;

    IDirectoryObjectPtr GetDirectoryObject() const override;
    void Rename(const std::wstring & newName) override;
    void UploadStringAttr(const std::wstring & attrName, const std::wstring & attrVal) override;
    std::wstring DownloadStringAttr(const std::wstring & attrName) override;

    void UploadBinaryAttr(const std::wstring & attrName, const BinaryAttrVal & bav) override;
    BinaryAttrVal DownloadBinaryAttr(const std::wstring & attrName) override;

private:
    std::wstring _dn;
    bool _connected = false;
};

}   // namespace adbook
