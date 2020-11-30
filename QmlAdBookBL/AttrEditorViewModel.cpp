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
#include "AttrTraits.h"
#include "AttrEditorViewModel.h"
#include "ADependencyResolver.h"

namespace qmladbook
{
    AttrEditorViewModel::AttrEditorViewModel(QObject *parent)
        : QObject(parent)
    {
    }

    AttrEditorViewModel::~AttrEditorViewModel() = default;

    QString AttrEditorViewModel::currentValue() {
        return _currentValue;
    }

    void AttrEditorViewModel::setCurrentValue(QString value) {
        _currentValue = value;
        emit currentValueChanged();
    }

    QString AttrEditorViewModel::newValue() {
        return _newValue;
    }

    void AttrEditorViewModel::setNewValue(QString value) {
        _newValue = value;
        auto dataExchanger =  GetDependencyResolver()->GetDataExchanger();
        dataExchanger->Set(ADataExchanger::SenderId::AttrEditor, _newValue);
        emit newValueChanged();
    }

    QString AttrEditorViewModel::ldapName() {
        return _ldapName;
    }

    void AttrEditorViewModel::setLdapName(QString value) {
        _ldapName = value;
        emit ldapNameChanged();
        emit attrDescriptionChanged();
    }

    QString AttrEditorViewModel::uiName() {
        return _uiName;
    }

    void AttrEditorViewModel::setUiName(QString value) {
        _uiName = value;
        emit uiNameChanged();
        emit attrDescriptionChanged();
    }

    QString AttrEditorViewModel::attributeOid() {
        return _attributeOid;
    }

    void AttrEditorViewModel::setAttributeOid(QString value)
    {
        _attributeOid = value;
        emit attributeOidChanged();
        emit attrDescriptionChanged();
    }

    unsigned int AttrEditorViewModel::maxLength() {
        return _maxLength;
    }

    void AttrEditorViewModel::setMaxLength(unsigned int value) {
        _maxLength = value;
        emit maxLengthChanged();
    }

    QString AttrEditorViewModel::attrDescription()
    {
        QString description = tr("Display name: ") + uiName() + '\n' +
            tr("LDAP name: ") + ldapName() + '\n' +
            tr("OID: ") + attributeOid() + '\n' +
            tr("Maximum length: %1").arg(maxLength());
        return description;
    }

    void AttrEditorViewModel::load(ContactDetailsModel * detailsModel, int row)
    {
        _attrId = detailsModel->GetAttrId(row);
        auto & attrTraits = AttrTraits::Instance();
        setMaxLength(attrTraits.GetMaxSizeInBytes(_attrId));
        setCurrentValue(detailsModel->GetAttrValue(row));
        setNewValue(detailsModel->GetAttrValue(row));
        setUiName(detailsModel->GetAttrUiName(row));
        setLdapName(attrTraits.GetLdapAttrName(_attrId));
        setAttributeOid(attrTraits.GetAttrOID(_attrId));
        setOwnerDn(detailsModel->GetAttrValue(adbook::Attributes::Dn));
    }

    QString AttrEditorViewModel::ownerDn()
    {
        return _ownerDn;
    }

    void AttrEditorViewModel::setOwnerDn(QString value)
    {
        _ownerDn = value;
        emit ownerDnChanged();
    }
}
