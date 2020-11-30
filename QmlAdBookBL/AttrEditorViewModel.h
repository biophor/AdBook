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
#include "ContactDetailsModel.h"
#include "qmladbookbl_global.h"

namespace qmladbook
{

    class QMLADBOOKBL_EXPORT AttrEditorViewModel : public QObject
    {
        Q_OBJECT
    public:
        Q_PROPERTY(QString currentValue READ currentValue WRITE setCurrentValue NOTIFY currentValueChanged);
        Q_INVOKABLE QString currentValue();
        Q_INVOKABLE void setCurrentValue(QString value);

        Q_PROPERTY(QString newValue READ newValue WRITE setNewValue NOTIFY newValueChanged);
        Q_INVOKABLE QString newValue();
        Q_INVOKABLE void setNewValue(QString value);

        Q_PROPERTY(QString ldapName READ ldapName WRITE setLdapName NOTIFY ldapNameChanged);
        Q_INVOKABLE QString ldapName();
        Q_INVOKABLE void setLdapName(QString value);

        Q_PROPERTY(QString ownerDn READ ownerDn WRITE setOwnerDn NOTIFY ownerDnChanged);
        Q_INVOKABLE QString ownerDn();
        Q_INVOKABLE void setOwnerDn(QString value);

        Q_PROPERTY(QString uiName READ uiName WRITE setUiName NOTIFY uiNameChanged);
        Q_INVOKABLE QString uiName();
        Q_INVOKABLE void setUiName(QString value);

        Q_PROPERTY(QString attributeOid READ attributeOid WRITE setAttributeOid NOTIFY attributeOidChanged);
        Q_INVOKABLE QString attributeOid();
        Q_INVOKABLE void setAttributeOid(QString value);

        Q_PROPERTY(unsigned int maxLength READ maxLength WRITE setMaxLength NOTIFY maxLengthChanged);
        Q_INVOKABLE unsigned int maxLength();
        Q_INVOKABLE void setMaxLength(unsigned int value);

        Q_PROPERTY(QString attrDescription READ attrDescription NOTIFY attrDescriptionChanged);
        Q_INVOKABLE QString attrDescription();

        Q_INVOKABLE void load(ContactDetailsModel * detailsModel, int row);

    signals:
        void ownerDnChanged();
        void currentValueChanged();
        void newValueChanged();
        void ldapNameChanged();
        void uiNameChanged();
        void attributeOidChanged();
        void maxLengthChanged();
        void attrDescriptionChanged();

    public:
        AttrEditorViewModel(QObject *parent = nullptr);
        virtual ~AttrEditorViewModel();

    private:
        adbook::Attributes::AttrId _attrId{ adbook::Attributes::AttrId::AttrIdEnd };
        QString _currentValue{""}, _newValue{ "" }, _attributeOid{ "" }, _uiName{ "" }, _ldapName{ "" }, _ownerDn;
        unsigned int _maxLength{10};
    };

}