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
#include "error.h"
#include "AdPersonDesc.h"

namespace adbook
{

template <class T>
inline T GetAttrVal(const LdapAttrName & an, const std::map<LdapAttrName, T> & m)
{
    T ret;
    BOOST_ASSERT(!an.empty());
    auto i = m.find(an);
    if (i != m.end())
    {
        ret = i->second;
    }
    return ret;
}

const wchar_t * AdPersonDesc::GetStringAttrPtr(const LdapAttrName & an) const
{
    auto attrIter = stringAttrs_.find(an);
    if (attrIter == stringAttrs_.cend()) {
        throw HrError(E_INVALIDARG, L"an param", __FUNCTIONW__);
    }
    return attrIter->second.c_str();
}

const BYTE * AdPersonDesc::GetBinaryAttrPtr(const LdapAttrName & an, size_t & attrSize) const
{
    auto attrIter = binaryAttrs_.find(an);
    if (attrIter == binaryAttrs_.cend()) {
        throw HrError(E_INVALIDARG, L"an param", __FUNCTIONW__);
    }
    if (attrIter->second.empty())
    {
        attrSize = 0;
        return nullptr;
    }
    attrSize = attrIter->second.size();
    return &attrIter->second[0];
}

StringAttrVal AdPersonDesc::GetStringAttr(const LdapAttrName & an) const
{
    return GetAttrVal(an, stringAttrs_);
}

BinaryAttrVal AdPersonDesc::GetBinaryAttr(const LdapAttrName & an) const
{
    return GetAttrVal(an, binaryAttrs_);
}

bool AdPersonDesc::IsAttributeSet(const LdapAttrName & an) const
{
    BOOST_VERIFY(!an.empty());
    return (stringAttrs_.find(an) != stringAttrs_.end()) || (binaryAttrs_.find(an) != binaryAttrs_.end());
}

bool AdPersonDesc::IsAttributeWritable(const Attributes::AttrId id) const noexcept
{
    return (writableAttributes_.find(id) != writableAttributes_.end());
}

StringAttrVal AdPersonDesc::GetDn() const
{
    return GetStringAttr(AdAttrDn);
}

AdPersonDesc::AttrIds AdPersonDesc::GetWritableAttributes() const
{
    return writableAttributes_;
}

bool AdPersonDesc::LexicographicalCompareStringAttrs(const AdPersonDesc & apd, Attributes::AttrId id) const
{
    const LdapAttrName ldapAttrName = Attributes::GetInstance().GetLdapAttrName(id);
    return LexicographicalCompareStringAttrs(apd, ldapAttrName);
}

bool AdPersonDesc::LexicographicalCompareStringAttrs(const AdPersonDesc & apd, const std::wstring & ldapAttrName) const
{
    auto li = stringAttrs_.find(ldapAttrName);
    auto ri = apd.stringAttrs_.find(ldapAttrName);
    bool lNotFound = li == stringAttrs_.cend();
    bool rNotFound = ri == apd.stringAttrs_.cend();
    if (lNotFound && rNotFound) {
        return false;
    }
    if (lNotFound) {
        return true;
    }
    if (rNotFound) {
        return false;
    }
    return std::lexicographical_compare(
        li->second.cbegin(), li->second.cend(),
        ri->second.cbegin(), ri->second.cend()
    );
}

}   // namespace adbook
