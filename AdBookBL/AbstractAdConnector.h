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

namespace adbook
{
class ADBOOKBL_API AbstractAdConnector
{
public:
    virtual ~AbstractAdConnector() = 0;

    virtual void Connect (
        const ConnectionParams & connectionParams
    ) = 0;

    virtual void Connect (
        const ConnectionParams & connectionSettings,
        const std::wstring & distinguishedName
    ) = 0;

    virtual void Disconnect() = 0;

    virtual bool IsConnected() const = 0;

    virtual void Rename (
        const std::wstring & newName
    ) = 0;

    virtual void UploadStringAttr (
        const std::wstring & attrName,
        const std::wstring & attrVal
    ) = 0;

    virtual std::wstring DownloadStringAttr (
        const std::wstring & attrName
    ) = 0;

    virtual void UploadBinaryAttr (
        const std::wstring & attrName,
        const BinaryAttrVal & bav
    ) = 0;

    virtual BinaryAttrVal DownloadBinaryAttr (
        const std::wstring & attrName
    ) = 0;
};

}   // namespace adbook