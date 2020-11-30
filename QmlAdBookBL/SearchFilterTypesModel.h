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
#include "SearchFilterType.h"
#include "qmladbookbl_global.h"

namespace qmladbook
{
    class QMLADBOOKBL_EXPORT SearchFilterTypesModel : public QAbstractListModel
    {
        Q_OBJECT

    public:
        SearchFilterTypesModel(QObject *parent = nullptr);
        SearchFilterType Get(int row) const;

    // Qt/QML
        int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
        QHash<int, QByteArray> roleNames() const override;

        static SearchFilterType GetTypeById(SearchFilterTypeId id);

    private:
        static void AddFilterType(SearchFilterTypeId id, const QString & uiName);
        static void AddFilterType(SearchFilterTypeId id, adbook::Attributes::AttrId attrId);
        static void OneTimeInitialization();
    private:
        static QVector<SearchFilterType> filterTypes;
        static std::once_flag initializationCompletted;
    };
}

