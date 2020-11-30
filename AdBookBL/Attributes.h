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


#pragma once

#include "AdBookBLExport.h"
#include "shared.h"

namespace adbook
{

class ADBOOKBL_API Attributes
{
public:
    enum AttrId : uint8_t {
        CommonName, Email, Company, Title, Department, State, EmpId,
        SurName, GivenName, DisplayName, WorkPhone, HomePhone, MobilePhone,
        Locality, ThumbnailPhoto, Dn, AttrIdEnd
    };


    static Attributes & GetInstance()   // we need only one instance
    {
        static Attributes instance;
        return instance;
    }
    size_t GetAttrCount() const noexcept;

    bool IsString(const AttrId id) const;
    bool IsEditable( const AttrId id ) const;
    bool IsEditableString( const AttrId id ) const;
    size_t GetAttrMaxLength(const AttrId id) const;
    std::wstring GetAttrOid( const AttrId id ) const;
    std::wstring GetUiAttrName( const AttrId id ) const;
    std::wstring GetLdapAttrName( const AttrId id ) const;
    const wchar_t * GetLdapAttrNamePtr( const AttrId id ) const;

    AttrId GetAttrId( const wchar_t * attrName ) const;

    size_t GetTextAttrMaxLength() const;
    size_t GetBinaryAttrMaxLength() const;

    std::vector<WcharBuf> GetAdsiComplientAttrNames() const;
    const std::list<std::wstring> & GetLdapAttrNames() const;
    const std::vector<AttrId> & GetAttrIds() const;

    bool IsAttrSupported( const AttrId attrId ) const noexcept;
    bool IsAttrSupported( const wchar_t * ldapAttrName ) const;

    static void PreExitUnload();    // to prevent false memory leaks warnings
private:
    Attributes();
    Attributes(const Attributes &) = delete;
    Attributes(Attributes &&) = delete;
    Attributes & operator = (const Attributes &) = delete;
    Attributes & operator = (Attributes &&) = delete;
private:
    static void PopulateTables();

private:
    struct LdapAttr
    {
        LdapAttr(LPCWSTR attrName, UINT attrGuiId, bool attrHasSingleValue, size_t attrMaxLen,
                LPCWSTR attrObjectId, bool attrIsString, bool attrIsEditable) :
            ldapAttrName(attrName),
            uiIds(attrGuiId),
            isSingleValued(attrHasSingleValue),
            maxLen(attrMaxLen),
            attrOid(attrObjectId),
            isString(attrIsString),
            isEditable(attrIsEditable) {}
        const wchar_t * ldapAttrName;
        const size_t maxLen;
        const wchar_t * attrOid;
        const unsigned int uiIds;
        const bool isSingleValued;
        const bool isString;
        const bool isEditable;  // attribute value can be change directly
    };

private:
    static std::map<Attributes::AttrId, LdapAttr> mainAttrInfo_;
    static std::vector<AttrId> attrIds_;
    static std::list<std::wstring> ldapAttrNames_;
    static std::map<const std::wstring, Attributes::AttrId> nameToIdMap_;
};

}
