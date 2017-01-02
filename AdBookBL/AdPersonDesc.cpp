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

StringAttrVal AdPersonDesc::GetStringAttr(const LdapAttrName & an) const
{
    return GetAttrVal(an, stringAttrs_);
}

StringAttrVal AdPersonDesc::GetStringAttr(Attributes::AttrId id) const
{
    return GetStringAttr(Attributes::GetInstance().GetLdapAttrName(id));
}

BinaryAttrVal AdPersonDesc::GetBinaryAttr(const LdapAttrName & an) const
{
    return GetAttrVal(an, binaryAttrs_);
}

BinaryAttrVal AdPersonDesc::GetBinaryAttr(Attributes::AttrId id) const
{
    return GetBinaryAttr(Attributes::GetInstance().GetLdapAttrName(id));
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
    return std::lexicographical_compare (
        li->second.cbegin(), li->second.cend(),
        ri->second.cbegin(), ri->second.cend()
    );
}

}   // namespace adbook
