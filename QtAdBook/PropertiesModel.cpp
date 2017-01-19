// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/*
Copyright (C) 2015-2017 Goncharov Andrei.

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
#include "PropertiesModel.h"

QList<PropertiesModel::ColumnDef> PropertiesModel::_columns = {
    { AttrNameColId, QObject::tr("Attribute") },
    { AttrValueColId, QObject::tr("Value") }
};

PropertiesModel::PropertiesModel(QObject *parent)
    : QStandardItemModel(parent)
{
    auto colNames = GetColumnNames();
    setColumnCount(colNames.size());
    setHorizontalHeaderLabels(colNames);

    auto & attrInst = adbook::Attributes::GetInstance();
    auto attrIds = attrInst.GetAttrIds();
    for (auto attrId : attrIds) {
        if (attrInst.IsEditableString(attrId)) {
            QString uiAttrName = QString::fromStdWString(attrInst.GetUiAttrName(attrId));
            QStandardItem * attrNameItem = new QStandardItem(uiAttrName);            
            QStandardItem * attrValueItem = new QStandardItem();
            attrValueItem->setData(attrId);
            appendRow(QList<QStandardItem*>({ attrNameItem, attrValueItem }));
        }
    }
}

PropertiesModel::~PropertiesModel()
{

}

void PropertiesModel::SetContact(const adbook::AdPersonDesc & apd)
{
    _apd = apd;
    DisplayProperties();
}

void PropertiesModel::DisplayProperties()
{        
    const int numRows = rowCount();
    for (int i = 0; i < numRows; ++i) {
        auto attrId = static_cast<adbook::Attributes::AttrId>(item(i, AttrValueColId)->data().toInt());
        const QString attrValue = QString::fromStdWString(_apd.GetStringAttr(attrId));
        item(i, AttrValueColId)->setText(attrValue);
    }
}

void PropertiesModel::Clear()
{
    _apd = adbook::AdPersonDesc();
    DisplayProperties();
}

adbook::Attributes::AttrId PropertiesModel::GetAttr(int row) const
{
    Q_ASSERT(row >= 0 && row < rowCount());
    auto attrId = static_cast<adbook::Attributes::AttrId>(item(row, AttrValueColId)->data().toInt());
    return attrId;
}

bool PropertiesModel::IsAttrWritable(int row) const
{
    return _apd.IsAttributeWritable(GetAttr(row));
}

adbook::AdPersonDesc PropertiesModel::GetContact()
{
    return _apd;
}