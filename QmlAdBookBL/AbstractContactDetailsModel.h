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
#include <QAbstractListModel>
#include "Contact.h"
#include "qmladbookbl_global.h"

namespace qmladbook
{
    class QMLADBOOKBL_EXPORT AbstractContactDetailsModel : public QAbstractListModel
    {
        Q_OBJECT

    public:
        AbstractContactDetailsModel(QObject *parent);
        virtual ~AbstractContactDetailsModel() = 0;

        enum Role {
            AttrNameRole = Qt::UserRole,
            AttrValueRole
        };

        virtual void SetContact(const Contact & contact) = 0;
        virtual QString GetAttrValue(adbook::Attributes::AttrId attrId) const = 0;
        virtual adbook::Attributes::AttrId GetAttrId(int row) const = 0;
        virtual QString GetAttrUiName(int row) const = 0;
        virtual QString GetAttrValue(int row) const = 0;
        virtual Contact GetContact() const = 0;
        virtual void Clear() = 0;
        virtual bool IsAttributeWriteable(int row) const = 0;
    };
}

