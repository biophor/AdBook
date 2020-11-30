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
    class QMLADBOOKBL_EXPORT ContactDetailsModel : public QAbstractListModel
    {
        Q_OBJECT

        struct StringAttribute {
            adbook::Attributes::AttrId attrId;
            QString uiName;
            QString value;
        };
    public:
        enum Role {
            AttrNameRole = Qt::UserRole,
            AttrValueRole
        };

        ContactDetailsModel(QObject *parent = nullptr);
        virtual ~ContactDetailsModel();

        void SetContact(const Contact & contact);
        QString GetAttrValue(adbook::Attributes::AttrId attrId) const;
        adbook::Attributes::AttrId GetAttrId(int row) const;
        QString GetAttrUiName(int row) const;
        QString GetAttrValue(int row) const;
        Contact GetContact() const;
        void Clear();

    // for QML
        Qt::ItemFlags flags(const QModelIndex &index) const;
        QVariant data(const QModelIndex& index, int nRole) const;

        int rowCount(const QModelIndex & parent = QModelIndex()) const;
        QVariant headerData(int nSection, Qt::Orientation orientation, int nRole) const;
        QHash<int, QByteArray> roleNames() const;

        // returns true is the attribute can be modified on the server
        Q_INVOKABLE bool isAttributeWriteable(int row) const;
    private:
        Contact _contact;
        QVector<StringAttribute> _attributes;
    };
}
