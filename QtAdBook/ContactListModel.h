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
#ifndef CONTACTLISTMODEL_H
#define CONTACTLISTMODEL_H

#include <QAbstractTableModel>


class ContactListModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    ContactListModel(
        std::shared_ptr<adbook::AbstractAdPersonDescKeeper> adPersonDescKeeper,
        QObject *parent = Q_NULLPTR
    );
    QVariant data(const QModelIndex & index, int nRole) const;
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    int columnCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant headerData(int nSection, Qt::Orientation orientation, int nRole = Qt::DisplayRole) const;
    bool insertRows(int row, int count, const QModelIndex & parent = QModelIndex());
    bool removeRows(int row, int count, const QModelIndex & parent = QModelIndex());
    virtual void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);
    void AddContacts(std::list<adbook::AdPersonDesc> && );
    void Clear();
    adbook::AdPersonDesc GetContact(int row) const;
    void SetContact(int row, const adbook::AdPersonDesc & apd);
    void Save(const adbook::ConnectionParams & connectionParams);
    void Load(const adbook::ConnectionParams & connectionParams);
private:
    mutable QMutex _contactsMutex;
    std::vector<adbook::AdPersonDesc> _contacts;

    std::shared_ptr<adbook::AbstractAdPersonDescKeeper> _adPersonDescKeeper;

    struct ColumnData {
        adbook::Attributes::AttrId attrId;
        QString headerLabel;
    };
    QList<ColumnData> _columnsData;
};

#endif // CONTACTLISTMODEL_H
