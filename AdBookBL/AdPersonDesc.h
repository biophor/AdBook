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
#include "AdBookBLExport.h"
#include "adsi.h"
#include "Attributes.h"

namespace adbook
{

class ADBOOKBL_API AdPersonDesc final
{
public:
    AdPersonDesc() = default;
    AdPersonDesc(const AdPersonDesc &) = default;
    AdPersonDesc(AdPersonDesc &&) noexcept = default;
    AdPersonDesc & operator = (const AdPersonDesc &) = default;
    AdPersonDesc & operator = (AdPersonDesc &&) noexcept = default;
    ~AdPersonDesc() = default;

    template <class StringAttrValType>
    void SetStringAttr (
        const LdapAttrName & an,
        StringAttrValType && sav
    );

    const wchar_t * GetStringAttrPtr (
        const LdapAttrName & an
    ) const;

    StringAttrVal GetStringAttr (
        const LdapAttrName & an
    ) const;

    // return true if the first range is lexicographically less than the second.
    bool LexicographicalCompareStringAttrs (
        const AdPersonDesc & apd,
        Attributes::AttrId id
    ) const;

    bool LexicographicalCompareStringAttrs (
        const AdPersonDesc & apd,
        const std::wstring & ldapAttrName
    ) const;

    template <class BinaryAttrValType>
    void SetBinaryAttr (
        const LdapAttrName & an,
        BinaryAttrValType && bav
    );

    const BYTE * GetBinaryAttrPtr (
        const LdapAttrName & an,
        size_t & attrSize
    ) const;

    BinaryAttrVal GetBinaryAttr (
        const LdapAttrName & an
    ) const;

    using AttrIds = std::set<Attributes::AttrId>;

    template <class AttrIdsType>
    void SetWritableAttributes (
        AttrIdsType && ait
    );

    AttrIds GetWritableAttributes() const;

    bool IsAttributeSet (
        const LdapAttrName & an
    ) const;

    bool IsAttributeWritable (
        const Attributes::AttrId id
    ) const noexcept;

    StringAttrVal GetDn() const;

    friend bool operator == ( const AdPersonDesc & adp1, const AdPersonDesc & adp2 ) {
        return
            adp1.writableAttributes_ == adp2.writableAttributes_ &&
            adp1.stringAttrs_ == adp2.stringAttrs_ &&
            adp1.binaryAttrs_ == adp2.binaryAttrs_;
    }
private:
    AttrIds writableAttributes_; // It's based on the AD attribute 'allowedAttributesEffective'
    std::map<LdapAttrName, StringAttrVal> stringAttrs_;
    std::map<LdapAttrName, BinaryAttrVal> binaryAttrs_;
};


template <class StringAttrValType>
void AdPersonDesc::SetStringAttr (
    const LdapAttrName & an,
    StringAttrValType && sav
)
{
    if (an.empty()) {
        throw HrError(E_INVALIDARG, L"an is empty", __FUNCTIONW__);
    }
    stringAttrs_[an] = std::forward<StringAttrValType>(sav);
}

template <class BinaryAttrValType>
void AdPersonDesc::SetBinaryAttr (
    const LdapAttrName & an,
    BinaryAttrValType && bav
)
{
    if (an.empty()) {
        throw HrError(E_INVALIDARG, L"an is empty", __FUNCTIONW__);
    }
    binaryAttrs_[an] = std::forward<BinaryAttrValType>(bav);
}

template <class AttrIdsType>
void AdPersonDesc::SetWritableAttributes (
    AttrIdsType && ait
)
{
    writableAttributes_ = std::forward<AttrIdsType>(ait);
}


}   // namespace adbook
