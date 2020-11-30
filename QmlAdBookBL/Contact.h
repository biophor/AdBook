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

#include <QObject>
#include <variant>
#include "qmladbookbl_global.h"

namespace qmladbook
{
    using AttributeValue = std::variant<std::monostate, QString, adbook::BinaryAttrVal>;

    class QMLADBOOKBL_EXPORT Contact
    {
    public:
        Contact();
        Contact(adbook::AdPersonDesc && adp);
        Contact(const Contact & that);
        Contact(Contact && that);
        virtual ~Contact();
        Contact & operator = (const Contact & that);
        Contact & operator = (Contact && that);

        AttributeValue GetAttr (
            adbook::Attributes::AttrId attrId
        ) const;

        void SetAttr (
            adbook::Attributes::AttrId attrId,
            const AttributeValue & contactAttr
        );

        QString GetDn() const;

        QString GetAttrAsString (
            adbook::Attributes::AttrId attrId
        ) const;

        adbook::BinaryAttrVal GetAttrAsBinary (
            adbook::Attributes::AttrId attrId
        ) const;

        operator adbook::AdPersonDesc() const;

        bool IsAttributeWritable(
            const adbook::Attributes::AttrId id
        ) const noexcept;

        friend bool operator == (const Contact & left, const Contact & right)
        {
            return left._cachedAttributes == right._cachedAttributes
                && left._adp == right._adp;
        }
    private:
        void ExtractAdpAttributesIntoCache();
        static AttributeValue GetDefaultValue( adbook::Attributes::AttrId attrId );
    private:
        std::map<adbook::Attributes::AttrId, AttributeValue> _cachedAttributes;// to increase reading speed
        adbook::AdPersonDesc _adp;
    };
}
