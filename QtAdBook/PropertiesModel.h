/*
Copyright (C) 2015-2020 Andrei Goncharov.

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

#ifndef PROPERTIESMODEL_H
#define PROPERTIESMODEL_H

#include <QStandardItemModel>

class PropertiesModel : public QStandardItemModel
{
    Q_OBJECT

public:
    enum ColumnId {
        AttrNameColId = 0, AttrValueColId = 1
    };

    PropertiesModel(QObject *parent);
    ~PropertiesModel();
    void SetContact(const adbook::AdPersonDesc & apd);
    adbook::AdPersonDesc GetContact();
    void Clear();
    bool IsAttrWritable(int row) const;
    adbook::Attributes::AttrId GetAttr(int row) const;
private:
    void DisplayProperties();
    struct ColumnDef {
        ColumnId colId;
        QString headerLabel;
    };
    static QStringList GetColumnNames() {
        QStringList rv;
        for (auto i : _columns) {
            rv.append(i.headerLabel);
        }
        return rv;
    }
    static QList<ColumnDef> _columns;
private:
    adbook::AdPersonDesc _apd;
};

#endif // PROPERTIESMODEL_H
