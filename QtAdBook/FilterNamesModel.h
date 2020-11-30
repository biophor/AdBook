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
#ifndef FILTERNAMESMODEL_H
#define FILTERNAMESMODEL_H

#include "FilterTypeItem.h"

class FilterNamesModel : public QStandardItemModel
{
public:
    FilterNamesModel(QObject *parent = Q_NULLPTR);

    FilterTypeItem * item(int row) const {
        return &dynamic_cast<FilterTypeItem &>(*QStandardItemModel::item(row, 0));
    }
};


#endif // FILTERNAMESMODEL_H
