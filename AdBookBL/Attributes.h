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
#include "adsi.h"

namespace adbook
{

class ADBOOKBL_API Attributes
{
public:
    enum AttrId {
        CommonName, Email, Company, Title, Department, State, EmpId,
        SurName, GivenName, DisplayName, WorkPhone, HomePhone, MobilePhone, Locality, thumbnailPhoto, dn, AttrIdEnd
    };

    static Attributes & GetInstance()
    {
        static Attributes instance;
        return instance;
    }
    bool IsEditable(const AttrId id) const;
    bool IsString(const AttrId id) const;
    bool IsEditableString(const AttrId id) const;
    std::wstring GetAttrOid(const AttrId id) const;
    size_t GetAttrCount() const noexcept;
    std::wstring GetUiAttrName(const AttrId id) const;
    std::wstring GetLdapAttrName(const AttrId id) const;
    size_t GetAttrMaxLength(const AttrId id) const;
    size_t GetTextAttrMaxLength() const;
    size_t GetBinaryAttrMaxLength() const;
    AttrId GetAttrId(const wchar_t * attrName) const;
    std::vector<WcharBuf> GetWritableLdapAttrNames() const;
    std::list<std::wstring> GetLdapAttrNames() const;
    std::vector<AttrId> GetAttrIds() const;
    bool IsAttrSupported(const AttrId attrId) const noexcept;
    bool IsAttrSupported(const wchar_t * ldapAttrName) const;

public:
    Attributes(const Attributes &) = delete;
    Attributes(Attributes &&) = delete;
    void operator = (const Attributes &) = delete;
    void operator = (Attributes &&) = delete;
private:
    Attributes();
};

}
