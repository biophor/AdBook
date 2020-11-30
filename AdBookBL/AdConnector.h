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
#include "adsi.h"
#include "AdBookBLExport.h"
#include "ConnectionParams.h"
#include "AbstractAdConnector.h"

namespace adbook
{

// AdConnector is used to read/write Active Directory objects' attributes.
class ADBOOKBL_API AdConnector: public AbstractAdConnector
{
public:
    explicit AdConnector();
    virtual ~AdConnector();

    AdConnector(const AdConnector &) = delete;
    AdConnector(AdConnector&&) = delete;
    AdConnector & operator = (const AdConnector&) = delete;
    AdConnector & operator - (AdConnector&&) = delete;

    void Connect (
        const ConnectionParams & connectionParams
    ) override;

    void Connect (
        const ConnectionParams & connectionSettings,
        const std::wstring & distinguishedName
    ) override;

    void Disconnect() override;
    bool IsConnected() const override;

    void Rename (
        const std::wstring & newName
    ) override;

    void UploadStringAttr (
        const std::wstring & attrName,
        const std::wstring & attrVal
    ) override;

    std::wstring DownloadStringAttr (
        const std::wstring & attrName
    ) override;

    void UploadBinaryAttr (
        const std::wstring & attrName,
        const BinaryAttrVal & bav
    ) override;

    BinaryAttrVal DownloadBinaryAttr (
        const std::wstring & attrName
    )override;

    IDirectoryObjectPtr GetDirectoryObject() const;

private:
    std::wstring GetLdapPath(); // https://docs.microsoft.com/en-us/windows/win32/adsi/ldap-adspath
    std::wstring GetRDN();  // RDN - relative distinguished name
    std::wstring GetParentLdapPath();
    IDirectoryObjectPtr GetParentObject();

    void GetObjectAttributesWrp (
        LPWSTR * attributeNames,
        DWORD numberAttributes,
        PADS_ATTR_INFO * attributeEntries,
        DWORD * numAttributesReturned
    );

    void SetObjectAttributesWrp (
        PADS_ATTR_INFO attributeEntries,
        DWORD numAttributes,
        DWORD * numAttributesModified
    );

    void CheckAttrNameByClearingAttribute (
        const std::wstring & attrName,
        ADSTYPE attrYype
    );

private:
    IDirectoryObjectPtr _objectPtr;
    ConnectionParams _connectionParams;
    std::wstring _distinguishedName;    // https://ldap.com/ldap-dns-and-rdns/
};

}   // namespace adbook

