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
#include "adsi.h"
#include "AdBookBLExport.h"
#include "ConnectionParams.h"

namespace adbook
{

class ADBOOKBL_API AdConnector
{
public:
    AdConnector(const ConnectionParams & cs);
    AdConnector(const ConnectionParams & cs, const std::wstring & dn);
    ~AdConnector();

    void Connect();
    void Disconnect();
    bool IsConnected() const;
    std::wstring GetLdapPath();
    std::wstring GetRDN();

    IDirectoryObjectPtr GetParentObject();
    IDirectoryObjectPtr GetDirectoryObject() const;
    void Rename(const std::wstring & newName);
    void UploadStringAttr(const std::wstring & attrName, const std::wstring & attrVal);
    std::wstring DownloadStringAttr(const std::wstring & attrName);

    void UploadBinaryAttr(const std::wstring & attrName, const BinaryAttrVal & bav);
    BinaryAttrVal DownloadBinaryAttr(const std::wstring & attrName);

private:
    IDirectoryObjectPtr objectPtr_;
    ConnectionParams cs_;
    std::wstring dn_;
};

}   // namespace adbook
