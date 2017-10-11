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
#ifndef FILTERLISTMODEL_H
#define FILTERLISTMODEL_H

#include "AppSettings.h"
#include "FilterTypeItem.h"
#include "FilterConditionItem.h"

class FilterListModel : public QStandardItemModel
{
    Q_OBJECT
public:
    FilterListModel(AppSettings & appSettings, QObject *parent = Q_NULLPTR);

    int FindFilter(FilterTypeItem * filterTypeItem, FilterConditionItem * conditionItem, 
        const QString & filterValue
    );

    FilterTypeItem * GetFilterType(int r) {
        return &dynamic_cast<FilterTypeItem &>(*item(r, FilterNameColId));
    }
    FilterConditionItem * GetFilterCondition(int r) {
        return &dynamic_cast<FilterConditionItem &>(*item(r, ConditionColId));
    }
    QString GetFilterValue(int r) {
        return item(r, ValueColId)->text();
    }
    int AddFilter(FilterTypeItem * filterTypeItem, FilterConditionItem * conditionItem, 
        const QString & filterValue
    );
    std::wstring ConstructLdapRequest(bool AllConditionsShouldBeMet);
    QStringList GetFilterValues();

    void SaveState();
    void LoadState();
private:    
    void LoadState(FilterCode filterCode, const QString & filterValue, FilterCondition condition);
    
    enum ColumnId { 
        FilterNameColId = 0, ConditionColId = 1, ValueColId = 2 
    };
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
    AppSettings & _appSettings;    
};

#endif // FILTERLISTMODEL_H
