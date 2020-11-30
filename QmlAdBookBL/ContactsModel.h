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
#include "AContactsModel.h"
#include "qmladbookbl_global.h"

namespace qmladbook
{
    class QMLADBOOKBL_EXPORT ContactsModel : public AContactsModel
    {
        Q_OBJECT

    public:
        ContactsModel(QObject *parent = nullptr);
        ~ContactsModel();

        enum Role {
            CommonNameRole = Qt::UserRole, SurNameRole, GivenNameRole, DisplayNameRole,
            EmailRole, CompanyRole, TitleRole, DepartmentRole, EmployeeIDRole,
            WorkPhoneRole, HomePhoneRole, MobilePhoneRole,
            LocalityOrCityRole, StateOrProvinceRole
        };

        void Clear() override;
        void AddContact(const Contact & contact) override;
        void AddContact(Contact && contact) override;

        const Contact & GetContact(int row) const override;
        void SetContact(int row, const Contact & contact) override;

   // Qt/QML
        QVariant data(const QModelIndex& index, int nRole) const override;
        int rowCount(const QModelIndex& parent = QModelIndex()) const override;
        QVariant headerData(int nSection, Qt::Orientation orientation, int nRole = Qt::DisplayRole) const override;
        Qt::ItemFlags flags(const QModelIndex &index) const override;
        bool insertRows(int nRow, int nCount, const QModelIndex& parent = QModelIndex()) override;
        bool removeRows(int nRow, int nCount, const QModelIndex& parent = QModelIndex()) override;
        QHash<int, QByteArray> roleNames() const override;

        Q_INVOKABLE void sort(QString roleName, int sortOrder);

        friend bool operator == (const ContactsModel & model1, const ContactsModel & model2) {
            return model1._contacts == model2._contacts;
        }
    private:
        int rowCountInternal() const;
        static adbook::Attributes::AttrId RoleToAttrId(Role);
        Role RoleNameToRole(const QString & roleName);
    private:
        QVector<Contact> _contacts;
        mutable std::mutex _mutex;
    };
}
