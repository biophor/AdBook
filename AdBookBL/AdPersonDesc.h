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
#include "Attributes.h"

namespace adbook
{

class ADBOOKBL_API AdPersonDesc
{
public:
    AdPersonDesc() = default;
    AdPersonDesc(const AdPersonDesc &) = default;
    AdPersonDesc(AdPersonDesc &&) = default;
    AdPersonDesc & operator = (const AdPersonDesc &) = default;
    AdPersonDesc & operator = (AdPersonDesc &&) = default;
    ~AdPersonDesc() = default;

    template <class StringAttrValType>
    void SetStringAttr(const LdapAttrName & an, StringAttrValType && sav);
    
    const wchar_t * GetStringAttrPtr(const LdapAttrName & an) const;
    StringAttrVal GetStringAttr(const LdapAttrName & an) const;
    StringAttrVal GetStringAttr(Attributes::AttrId id) const;    
    bool LexicographicalCompareStringAttrs(const AdPersonDesc & apd, Attributes::AttrId id) const;
    
    template <class BinaryAttrValType>
    void SetBinaryAttr(const LdapAttrName & an, BinaryAttrValType && bav);

    const BYTE * GetBinaryAttrPtr(const LdapAttrName & an, size_t & attrSize) const;
    BinaryAttrVal GetBinaryAttr(const LdapAttrName & an) const;
    BinaryAttrVal GetBinaryAttr(Attributes::AttrId id) const;

    using AttrIds = std::set<Attributes::AttrId>;

    template <class AttrIdsType>
    void SetWritableAttributes(AttrIdsType && ait);

    AttrIds GetWritableAttributes() const;

    bool IsAttributeSet(const LdapAttrName & an) const;
    bool IsAttributeWritable(const Attributes::AttrId id) const noexcept;

    StringAttrVal GetDn() const;

private:
    AttrIds writableAttributes_;
    std::map<LdapAttrName, StringAttrVal> stringAttrs_;
    std::map<LdapAttrName, BinaryAttrVal> binaryAttrs_;
};

template <class StringAttrValType>
void AdPersonDesc::SetStringAttr(const LdapAttrName & an, StringAttrValType && sav)
{
    BOOST_ASSERT(!an.empty());
    stringAttrs_[an] = std::forward<StringAttrValType>(sav);
}

template <class BinaryAttrValType>
void AdPersonDesc::SetBinaryAttr(const LdapAttrName & an, BinaryAttrValType && bav)
{
    BOOST_ASSERT(!an.empty());
    binaryAttrs_[an] = std::forward<BinaryAttrValType>(bav);
}

template <class AttrIdsType>
void AdPersonDesc::SetWritableAttributes(AttrIdsType && ait)
{
    writableAttributes_ = std::forward<AttrIds>(ait);
}

}   // namespace adbook
