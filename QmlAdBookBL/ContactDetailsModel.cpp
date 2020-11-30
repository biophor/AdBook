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
#include "ContactDetailsModel.h"

namespace qmladbook
{
    ContactDetailsModel::ContactDetailsModel(QObject *parent)
        : QAbstractListModel(parent)
    {
        auto & attrTraits = adbook::Attributes::GetInstance();
        const auto & attrNames = attrTraits.GetLdapAttrNames();

        for (auto iter = attrNames.cbegin(); iter != attrNames.cend(); ++iter)
        {
            const auto attrId = attrTraits.GetAttrId(iter->c_str());
            if (!attrTraits.IsString(attrId)) {
                continue;
            }
            const std::wstring attrUiName = attrTraits.GetUiAttrName(attrId);

            StringAttribute stringAttribute;
            stringAttribute.attrId = attrId;
            stringAttribute.uiName = QString::fromStdWString(attrUiName);
            _attributes.append(stringAttribute);
        }
    }

    ContactDetailsModel::~ContactDetailsModel() = default;

    QHash<int, QByteArray> ContactDetailsModel::roleNames() const
    {
        QHash<int, QByteArray> rn{
            {AttrNameRole , "AttrName"},
            {AttrValueRole, "AttrValue"}
        };
        return rn;
    }

    int ContactDetailsModel::rowCount(const QModelIndex& parent) const
    {
        Q_UNUSED(parent);
        return _attributes.count();
    }

    QVariant ContactDetailsModel::data(const QModelIndex& index, int nRole) const
    {
        if (!index.isValid()) {
            return {};
        }
        if (index.row() < 0 || index.row() >= _attributes.size()) {
            return {};
        }
        const auto & attribute = _attributes.at(index.row());

        switch (nRole)
        {
        case AttrNameRole:
            return attribute.uiName;

        case AttrValueRole:
            return attribute.value;

        default:
            return {};
        }
    }

    QVariant ContactDetailsModel::headerData(int nSection, Qt::Orientation orientation, int nRole) const
    {
        Q_UNUSED(nSection);
        if (orientation != Qt::Horizontal) {
            return QVariant();
        }
        switch (nRole)
        {
        case AttrNameRole:
            return tr("Attribute");

        case AttrValueRole:
            return tr("Value");

        default:
            throw adbook::HrError(E_INVALIDARG, L"invalid nRole", __FUNCTIONW__);
        }
    }

    Qt::ItemFlags ContactDetailsModel::flags(const QModelIndex &index) const
    {
        Qt::ItemFlags flags = QAbstractListModel::flags(index);
        return flags;
    }

    void ContactDetailsModel::SetContact(const Contact & contact)
    {
        _contact = contact;
        emit layoutAboutToBeChanged();
        for (auto & attr : _attributes)
        {
            AttributeValue attrValue = contact.GetAttr(attr.attrId);
            if (std::holds_alternative<QString>(attrValue)) {
                attr.value = std::get<QString>(attrValue);
            }
            else {
                attr.value = "";
            }
        }
        emit layoutChanged();
    }

    Contact ContactDetailsModel::GetContact() const
    {
        return _contact;
    }

    QString ContactDetailsModel::GetAttrValue(adbook::Attributes::AttrId attrId) const
    {
        for (auto && attr : _attributes) {
            if (attr.attrId == attrId) {
                return attr.value;
            }
        }
        return "";
    }

    adbook::Attributes::AttrId ContactDetailsModel::GetAttrId(int row) const
    {
        if (row < 0 || row >= _attributes.count()) {
            throw adbook::HrError(E_INVALIDARG, L"row", __FUNCTIONW__);
        }
        return _attributes.at(row).attrId;
    }

    QString ContactDetailsModel::GetAttrUiName(int row) const
    {
        if (row < 0 || row >= _attributes.count()) {
            throw adbook::HrError(E_INVALIDARG, L"row", __FUNCTIONW__);
        }
        return _attributes.at(row).uiName;
    }

    QString ContactDetailsModel::GetAttrValue(int row) const
    {
        if (row < 0 || row >= _attributes.count()) {
            throw adbook::HrError(E_INVALIDARG, L"row", __FUNCTIONW__);
        }
        return _attributes.at(row).value;
    }

    void ContactDetailsModel::Clear()
    {
        emit layoutAboutToBeChanged();
        for (auto & attr : _attributes) {
            attr.value.clear();
        }
        emit layoutChanged();
    }

    bool ContactDetailsModel::isAttributeWriteable(int row) const
    {
        if (row < 0 || row >= _attributes.count()) {
            throw adbook::HrError(E_INVALIDARG, L"row", __FUNCTIONW__);
        }
        const StringAttribute & attrInfo = _attributes.at(row);
        return _contact.IsAttributeWritable(attrInfo.attrId);
    }
}

