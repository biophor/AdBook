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

#include "stdafx.h"
#include "Contact.h"

namespace qmladbook
{
    Contact::Contact() = default;

    Contact::Contact(adbook::AdPersonDesc && adp) :
        _adp{ std::move(adp) }
    {
        ExtractAdpAttributesIntoCache();
    }

    Contact::Contact(const Contact & that) :
        _adp{ that._adp },
        _cachedAttributes{ that._cachedAttributes }
    { }

    Contact::Contact(Contact && that) :
        _adp{ std::move(that._adp) },
        _cachedAttributes{ std::move(that._cachedAttributes) }
    { }

    Contact::~Contact() = default;

    void Contact::ExtractAdpAttributesIntoCache()
    {
        static auto & attrTraits = adbook::Attributes::GetInstance();
        std::list<std::wstring> attrNames = attrTraits.GetLdapAttrNames();

        for (auto && attrName : attrNames)
        {
            const auto attrId = attrTraits.GetAttrId(attrName.c_str());

            if (attrTraits.IsString(attrId))
            {
                if (_adp.IsAttributeSet(attrName))
                {
                    const std::wstring attrValue = _adp.GetStringAttr(attrName);
                    _cachedAttributes[attrId] = QString::fromStdWString(attrValue);
                }
            }
            else
            {
                if (_adp.IsAttributeSet(attrName))
                {
                    const adbook::BinaryAttrVal binaryAttr = _adp.GetBinaryAttr(attrName);
                    _cachedAttributes[attrId] = binaryAttr;
                }
            }
        }
    }

    AttributeValue Contact::GetAttr( adbook::Attributes::AttrId attrId ) const
    {
        if (auto iter = _cachedAttributes.find(attrId); iter == _cachedAttributes.cend()) {
            return GetDefaultValue(attrId);
        }
        else {
            return iter->second;
        }
    }

    QString Contact::GetAttrAsString( adbook::Attributes::AttrId attrId ) const {
        return std::get<QString>(GetAttr(attrId));
    }

    adbook::BinaryAttrVal Contact::GetAttrAsBinary( adbook::Attributes::AttrId attrId ) const {
        return std::get<adbook::BinaryAttrVal>(GetAttr(attrId));
    }

    void Contact::SetAttr(adbook::Attributes::AttrId attrId, const AttributeValue & contactAttr)
    {
        _cachedAttributes[attrId] = contactAttr;

        auto & attrTraits = adbook::Attributes::GetInstance();
        auto attrName = attrTraits.GetLdapAttrName(attrId);

        if (attrTraits.IsString(attrId))
        {
            const std::wstring wAttrValue = std::get<QString>(contactAttr).toStdWString();
            _adp.SetStringAttr(attrName, wAttrValue);
        }
        else
        {
            const adbook::BinaryAttrVal attrValue = std::get<adbook::BinaryAttrVal>(contactAttr);
            _adp.SetBinaryAttr(attrName, attrValue);
        }

    }

    Contact & Contact ::operator = (const Contact & that)
    {
        if (this == &that) {
            return *this;
        }
        _cachedAttributes = that._cachedAttributes;
        _adp = that._adp;
        return *this;
    }

    Contact & Contact::operator = (Contact && that)
    {
        if (this == &that) {
            return *this;
        }
        _cachedAttributes = std::move(that._cachedAttributes);
        _adp = std::move(that._adp);
        return *this;
    }

    QString Contact::GetDn() const
    {
        AttributeValue av = Contact::GetAttr(adbook::Attributes::Dn);
        return std::get<QString>(av);
    }

    Contact::operator adbook::AdPersonDesc() const {
        return _adp;
    }

    bool Contact::IsAttributeWritable( const adbook::Attributes::AttrId id ) const noexcept {
        return _adp.IsAttributeWritable(id);
    }

    AttributeValue Contact::GetDefaultValue( adbook::Attributes::AttrId attrId )
    {
        static auto & attrTraits = adbook::Attributes::GetInstance();
        if (attrTraits.IsString(attrId)) {
            return QString{};
        }
        else {
            return adbook::BinaryAttrVal{};
        }
    }
}
