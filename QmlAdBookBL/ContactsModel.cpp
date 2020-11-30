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
#include "ContactsModel.h"

namespace qmladbook
{
    ContactsModel::ContactsModel(QObject *parent)
        : AContactsModel(parent)
    {
    }

    ContactsModel::~ContactsModel()
    {
    }

    void ContactsModel::Clear()
    {
        if (rowCount() == 0) {
            return;
        }
        beginRemoveRows(QModelIndex(), 0, _contacts.size() - 1);
        try {
            std::scoped_lock scopedLock(_mutex);
            _contacts.clear();
        }
        catch (const std::exception & e) {
            endRemoveRows();
            MY_TRACE_STDEXCEPTION(e);
            throw;
        }
        endRemoveRows();
    }

    void ContactsModel::AddContact(const Contact & contact)
    {
        const int nRow = rowCount();
        beginInsertRows(QModelIndex(), nRow, nRow);
        try {
            std::scoped_lock scopedLock(_mutex);
            _contacts.push_back(contact);
        }
        catch (const std::exception & e) {
            endInsertRows();
            MY_TRACE_STDEXCEPTION(e);
            throw;
        }
        endInsertRows();
    }

    void ContactsModel::AddContact(Contact && contact)
    {
        const int nRow = rowCount();
        beginInsertRows(QModelIndex(), nRow, nRow);
        try {
            std::scoped_lock scopedLock(_mutex);
            _contacts.push_back(std::move(contact));
        }
        catch (const std::exception & e) {
            endInsertRows();
            MY_TRACE_STDEXCEPTION(e);
            throw;
        }
        endInsertRows();
    }

    const Contact & ContactsModel::GetContact(int row) const
    {
        std::scoped_lock scopedLock(_mutex);

        if (row < 0 || row >= _contacts.size()) {
            throw adbook::HrError(E_INVALIDARG, L"row", __FUNCTIONW__);
        }
        return _contacts[row];
    }

    void ContactsModel::SetContact(int row, const Contact & contact)
    {
        {
            std::scoped_lock scopedLock(_mutex);
            if (row < 0 || row >= _contacts.size()) {
                throw adbook::HrError(E_INVALIDARG, L"row", __FUNCTIONW__);
            }
            _contacts[row] = contact;
        }
        QModelIndex idx = index(row);
        emit dataChanged(idx, idx);
    }

    QVariant ContactsModel::data(const QModelIndex& index, int nRole) const
    {
        if (!index.isValid()) {
            return QVariant();
        }
        std::scoped_lock scopedLock(_mutex);
        if (index.row() < 0 || index.row() >= _contacts.size()) {
            return QVariant();
        }
        const int row = index.row();
        const adbook::Attributes::AttrId attrId = RoleToAttrId(static_cast<Role>(nRole));

        if (!adbook::Attributes::GetInstance().IsString(attrId)) {
            throw adbook::HrError(E_INVALIDARG, L"role is invalid", __FUNCTIONW__);
        }
        const Contact & contact = _contacts[row];
        AttributeValue attr = contact.GetAttr(attrId);
        if (std::holds_alternative<QString>(attr)) {
            return std::get<QString>(attr);
        }
        return QString{ "" };
    }

    int ContactsModel::rowCount(const QModelIndex& parent) const
    {
        Q_UNUSED(parent);
        std::scoped_lock scopedLock(_mutex);
        return _contacts.count();

    }

    int ContactsModel::rowCountInternal() const {
        return _contacts.count();
    }

    QHash<int, QByteArray> ContactsModel::roleNames() const
    {
        std::initializer_list<std::pair<int, QByteArray>> roles = {
            {CommonNameRole, "CommonName"},
            {SurNameRole, "SurName"},
            {GivenNameRole, "GivenName"},
            {DisplayNameRole,"DisplayName"},
            {EmailRole, "Email"},
            {CompanyRole, "Company"},
            {TitleRole,"Title"},
            {DepartmentRole, "Department"},
            {EmployeeIDRole, "EmployeeID"},
            {WorkPhoneRole, "WorkPhone"},
            {HomePhoneRole,"HomePhone"},
            {MobilePhoneRole,"MobilePhone"},
            {LocalityOrCityRole, "Locality"},
            {StateOrProvinceRole, "StateOrProvince"}
        };
        return QHash<int, QByteArray>(roles);
    }

    Qt::ItemFlags ContactsModel::flags(const QModelIndex &index) const
    {
        Qt::ItemFlags flags = QAbstractListModel::flags(index);
        flags |= Qt::ItemNeverHasChildren;
        return flags;
    }

    bool ContactsModel::insertRows(int nRow, int nCount, const QModelIndex& parent)
    {
        if (parent.isValid()) {
            throw adbook::HrError(E_INVALIDARG, L"parent", __FUNCTIONW__);
        }
        beginInsertRows(QModelIndex(), nRow, nRow + nCount - 1);
        try {
            std::scoped_lock scopedLock(_mutex);
            for (int i = 0; i < nCount; ++i) {
                _contacts.insert(nRow, {});
            }
        }
        catch (const std::exception & e) {
            endInsertRows();
            MY_TRACE_STDEXCEPTION(e);
            throw;
        }
        endInsertRows();
        return true;
    }

    bool ContactsModel::removeRows(int nRow, int nCount, const QModelIndex& parent)
    {
        if (parent.isValid()) {
            throw adbook::HrError(E_INVALIDARG, L"parent", __FUNCTIONW__);
        }
        beginRemoveRows(QModelIndex(), nRow, nRow + nCount - 1);
        try {
            std::scoped_lock scopedLock(_mutex);
            for (int i = 0; i < nCount; ++i) {
                _contacts.removeAt(nRow);
            }
        }
        catch (const std::exception & e) {
            endRemoveRows();
            MY_TRACE_STDEXCEPTION(e);
            throw;
        }
        endRemoveRows();
        return true;
    }

    QVariant ContactsModel::headerData(int nSection, Qt::Orientation orientation, int nRole) const
    {
        Q_UNUSED(nSection);
        if (orientation != Qt::Horizontal) {
            return {};
        }
        auto attrId = RoleToAttrId(static_cast<Role>(nRole));
        QString headerTitle = QString::fromStdWString(adbook::Attributes::GetInstance().GetUiAttrName(attrId));
        return headerTitle;
    }

    adbook::Attributes::AttrId ContactsModel::RoleToAttrId(Role role)
    {
        switch (role) {
        case CommonNameRole:
            return adbook::Attributes::CommonName;
        case SurNameRole:
            return adbook::Attributes::SurName;
        case GivenNameRole:
            return adbook::Attributes::GivenName;
        case DisplayNameRole:
            return adbook::Attributes::DisplayName;
        case EmailRole:
            return adbook::Attributes::Email;
        case CompanyRole:
            return adbook::Attributes::Company;
        case TitleRole:
            return adbook::Attributes::Title;
        case DepartmentRole:
            return adbook::Attributes::Department;
        case EmployeeIDRole:
            return adbook::Attributes::EmpId;
        case WorkPhoneRole:
            return adbook::Attributes::WorkPhone;
        case HomePhoneRole:
            return adbook::Attributes::HomePhone;
        case MobilePhoneRole:
            return adbook::Attributes::MobilePhone;
        case LocalityOrCityRole:
            return adbook::Attributes::Locality;
        case StateOrProvinceRole:
            return adbook::Attributes::State;
        default:
            throw adbook::HrError(E_INVALIDARG, L"role", __FUNCTIONW__);
        }
    }

    ContactsModel::Role ContactsModel::RoleNameToRole(const QString & roleName)
    {
        QHash<int, QByteArray> rn = roleNames();
        for (auto iter = rn.keyValueBegin(); iter != rn.keyValueEnd(); ++iter) {
            if ((*iter).second == roleName) {
                return static_cast<Role>((*iter).first);
            }
        }
        throw adbook::HrError(E_INVALIDARG, L"roleName", __FUNCTIONW__);
    }

    void ContactsModel::sort(QString roleName, int sortOrder)
    {
        Qt::SortOrder lSortOrder = static_cast<Qt::SortOrder>(sortOrder);
        Role role = RoleNameToRole(roleName);
        adbook::Attributes::AttrId attrId = RoleToAttrId(role);

        emit layoutAboutToBeChanged();
        {
            std::scoped_lock scopedLock(_mutex);
            std::stable_sort(
                _contacts.begin(), _contacts.end(),
                [lSortOrder, attrId, this](const Contact & left, const Contact & right) {
                AttributeValue leftAttrValue = left.GetAttr(attrId);
                AttributeValue rightAttrValue = right.GetAttr(attrId);

                if (Qt::AscendingOrder == lSortOrder) {
                    return leftAttrValue < rightAttrValue;
                }
                else {
                    return rightAttrValue < leftAttrValue;
                }
            }
            );
        }
        emit layoutChanged();
    }
}
