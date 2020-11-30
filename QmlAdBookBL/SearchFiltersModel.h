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
#include "SearchFilter.h"
#include "qmladbookbl_global.h"

namespace qmladbook
{
    class QMLADBOOKBL_EXPORT SearchFiltersModel : public QAbstractListModel
    {
        Q_OBJECT

    public:
        SearchFiltersModel(QObject *parent = nullptr);

        enum Role {
            FilterTypeRole = Qt::UserRole,
            FilterRuleRole,
            FilterValueRole
        };

        bool Exists(const SearchFilter & searchFilter);
        void Add(const SearchFilter & searchFilter);
        void Remove(int row);
        const SearchFilter & Get(int row) const;
        void Set(int row, const SearchFilter & searchFilter);
        QVector<SearchFilter> GetFilters() const;
        void SetFilters(const QVector<SearchFilter> & filters);

        QByteArray GetSerializedFilters() const;
        void SetFilters(const QByteArray & serializedFilters);

    // Qt/QML
        QVariant data(const QModelIndex & index, int nRole) const override;
        int rowCount(const QModelIndex& parent = QModelIndex()) const override;
        QVariant headerData(int nSection, Qt::Orientation orientation, int nRole) const override;
        Qt::ItemFlags flags(const QModelIndex &index) const override;
        bool insertRows(int nRow, int nCount, const QModelIndex& parent = QModelIndex()) override;
        bool removeRows(int nRow, int nCount, const QModelIndex& parent = QModelIndex()) override;
        QHash<int, QByteArray> roleNames() const override;

    private:
        QVector<SearchFilter> _filters;
        static const quint32 _serializedFiltersVersion{ 1 };
    };
}
