// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
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

#include "stdafx.h"
#include "adsi.h"
#include "error.h"
#include "dllmain.h"
#include "resource.h"
#include "Attributes.h"


namespace adbook
{

std::map<const std::wstring, Attributes::AttrId> nameToIdMap;

struct LdapAttr
{
    LdapAttr(LPCWSTR n, UINT i, bool sv, size_t ml, LPCWSTR o, bool s, bool e) :
        ldapAttrName(n), uiIds(i), isSingleValued(sv), maxLen(ml), attrOid(o), isString(s), isEditable(e) {}
    const wchar_t * ldapAttrName;
    const size_t maxLen;
    const wchar_t * attrOid;
    const unsigned int uiIds;
    const bool isSingleValued;
    const bool isString;
    const bool isEditable;
};

std::map<Attributes::AttrId, LdapAttr> mainAttrInfo =
{
    { Attributes::CommonName, LdapAttr{ L"cn", IDS_ATTR_CN, true, 64, L"2.5.4.3", true, true } },
    { Attributes::Email, LdapAttr{ L"mail", IDS_ATTR_EMAIL, true, 256, L"0.9.2342.19200300.100.1.3", true, true } },
    { Attributes::Company, LdapAttr{ L"company", IDS_ATTR_COMPANY, true, 64, L"1.2.840.113556.1.2.146", true, true } },
    { Attributes::Title, LdapAttr{ L"title", IDS_ATTR_TITLE, true, 64, L"2.5.4.12", true, true } },
    { Attributes::Department, LdapAttr{ L"department", IDS_ATTR_DEPARTMENT, true, 64, L"1.2.840.113556.1.2.141", true, true } },
    { Attributes::State, LdapAttr{ L"st", IDS_ATTR_STATE_PROVINCE, true, 128, L"2.5.4.8", true, true } },
    { Attributes::EmpId, LdapAttr{ L"employeeID", IDS_ATTR_EMPLOYEE_ID, true, 16, L"1.2.840.113556.1.4.35", true, true } },
    { Attributes::SurName, LdapAttr{ L"sn", IDS_ATTR_SURNAME, true, 64, L"2.5.4.4", true, true } },
    { Attributes::GivenName, LdapAttr{ L"givenName", IDS_ATTR_GIVENNAME, true, 64, L"2.5.4.42", true, true } },
    { Attributes::DisplayName, LdapAttr{ L"displayName", IDS_ATTR_DISPLAYNAME, true, 256, L"1.2.840.113556.1.2.13", true, true } },
    { Attributes::WorkPhone, LdapAttr{ L"telephoneNumber", IDS_ATTR_TELEPHONE, true, 64, L"2.5.4.20", true, true } },
    { Attributes::HomePhone, LdapAttr{ L"homePhone", IDS_ATTR_HOMEPHONE, true, 64, L"0.9.2342.19200300.100.1.20", true, true } },
    { Attributes::MobilePhone, LdapAttr{ L"mobile", IDS_ATTR_MOBILE_PHONE, true, 64, L"0.9.2342.19200300.100.1.41", true, true } },
    { Attributes::Locality, LdapAttr{ L"l", IDS_ATTR_LOCALITY_CITY, true, 128, L"2.5.4.7", true, true } },
    { Attributes::ThumbnailPhoto, LdapAttr{ L"thumbnailPhoto", IDS_ATTR_THUMBNAIL_PHOTO, true, 102400, L"2.16.840.1.113730.3.1.35", false, true } },
    { Attributes::Dn, LdapAttr{ L"distinguishedName", IDS_ATTR_DN, true, 10240, L"2.5.4.49", true, false } }
};

bool Attributes::IsString(const AttrId id) const
{
    return mainAttrInfo.at(id).isString;
}

bool Attributes::IsEditable(const AttrId id) const
{
    return mainAttrInfo.at(id).isEditable;
}

bool Attributes::IsEditableString(const AttrId id) const
{
    const auto & x = mainAttrInfo.at(id);
    return x.isEditable && x.isString;
}

std::wstring Attributes::GetLdapAttrName(const AttrId id) const
{
    return std::wstring(mainAttrInfo.at(id).ldapAttrName);
}

const wchar_t * Attributes::GetLdapAttrNamePtr(const AttrId id) const
{
    return mainAttrInfo.at(id).ldapAttrName;
}

size_t Attributes::GetAttrMaxLength(const AttrId id) const
{
    return mainAttrInfo.at(id).maxLen;
}

size_t Attributes::GetTextAttrMaxLength() const
{
    size_t maxLen = 0;
    for (const auto & i : mainAttrInfo)
    {
        if (i.second.isString && maxLen < i.second.maxLen)
        {
            maxLen = i.second.maxLen;
        }
    }
    return maxLen;
}

size_t Attributes::GetBinaryAttrMaxLength() const
{
    size_t maxLen = 0;
    for (const auto & i : mainAttrInfo)
    {
        if (!i.second.isString && maxLen < i.second.maxLen)
        {
            maxLen = i.second.maxLen;
        }
    }
    return maxLen;
}

std::wstring Attributes::GetAttrOid(const AttrId id) const
{
    return std::wstring(mainAttrInfo.at(id).attrOid);
}

size_t Attributes::GetAttrCount() const noexcept
{
    return mainAttrInfo.size();
}

std::wstring Attributes::GetUiAttrName(const AttrId id) const
{
    wchar_t buf[256];
    if (!LoadStringW(moduleHandle, mainAttrInfo.at(id).uiIds, buf, _countof(buf)))
    {
        throw HrError(HRESULT_FROM_WIN32(ERROR_RESOURCE_NOT_FOUND), __FUNCTIONW__);
    }
    return buf;
}

Attributes::AttrId Attributes::GetAttrId(const wchar_t * attrName) const
{
    BOOST_ASSERT(attrName != nullptr);
    return nameToIdMap.at(attrName);
}

std::vector<WcharBuf> Attributes::GetWritableLdapAttrNames() const
{
    std::vector<WcharBuf> attrNames;
    for (const auto & i : mainAttrInfo)
    {
        const std::wstring tmp = i.second.ldapAttrName;
        WcharBuf buf(tmp.begin(), tmp.end());
        buf.push_back(0);
        attrNames.push_back(std::move(buf));
    }
    return attrNames;
}

const std::list<std::wstring> & Attributes::GetLdapAttrNames() const
{
    return ldapAttrNames_;    
}

const std::vector<Attributes::AttrId> & Attributes::GetAttrIds() const
{
    return attrIds_;
}

bool Attributes::IsAttrSupported(const AttrId attrId) const noexcept
{
    return (mainAttrInfo.find(attrId) != mainAttrInfo.end());
}

bool Attributes::IsAttrSupported(const wchar_t * ldapAttrName) const
{
    BOOST_ASSERT(ldapAttrName != nullptr);
    return (nameToIdMap.find(ldapAttrName) != nameToIdMap.end());
}

Attributes::Attributes()
{
    for (const auto & i : mainAttrInfo)
    {
        ldapAttrNames_.push_back(i.second.ldapAttrName);
        attrIds_.push_back(i.first);
        nameToIdMap.emplace(i.second.ldapAttrName, i.first);
    }    
}

void Attributes::PreExitUnload()
{
    nameToIdMap.clear(); // prevent messages about 'memory leaks'
    mainAttrInfo.clear();
}

}   // namespace adbook
