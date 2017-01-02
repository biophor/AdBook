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
#include "ContactListModel.h"

ContactListModel::ContactListModel(QObject * parent):
    QAbstractTableModel(parent), _contactsMutex(QMutex::Recursive)
{    
    auto & attrInst = adbook::Attributes::GetInstance();
    auto attrIds = attrInst.GetAttrIds();
    for (auto attrId : attrIds) {
        if (attrInst.IsEditableString(attrId)) {
            ColumnData cd;
            cd.headerLabel = QString::fromStdWString(attrInst.GetUiAttrName(attrId));
            cd.attrId = attrId;
            _columnsData.push_back(cd);            
        }
    }
}

QVariant ContactListModel::data(const QModelIndex & index, int nRole) const
{
    if (!index.isValid()) {
        return QVariant();
    }
    if (nRole == Qt::DisplayRole/* || nRole == Qt::ToolTipRole*/) {        
        QMutexLocker locker(&_contactsMutex);
        const auto & contact = _contacts.at(index.row());
        return QString::fromStdWString(contact.GetStringAttr(_columnsData.at(index.column()).attrId));
    }
    return QVariant();
}

int ContactListModel::rowCount(const QModelIndex & ) const
{
    return  boost::numeric_cast<int>(_contacts.size());
}

int ContactListModel::columnCount(const QModelIndex & ) const
{
    return _columnsData.size();
}

QVariant ContactListModel::headerData(int nSection, Qt::Orientation orientation, int nRole) const
{
    if (nRole != Qt::DisplayRole) {
        return QVariant();
    }
    if (orientation == Qt::Vertical) {
        return QAbstractTableModel::headerData(nSection, orientation, nRole);
    }
    return _columnsData.at(nSection).headerLabel;
}

bool ContactListModel::insertRows(int row, int count, const QModelIndex & parent)
{    
    try
    {
        if (0 == count) {
            return true;
        }
        const int first = row;
        const int last = row + count - 1;
        beginInsertRows(parent, first, last);
        QMutexLocker locker(&_contactsMutex);
        _contacts.insert(_contacts.begin()+row, count, adbook::AdPersonDesc());
    }
    catch (const std::exception &)
    {
        endInsertRows();
        return false;
    }
    endInsertRows();
    return true;
}

bool ContactListModel::removeRows(int row, int count, const QModelIndex & parent)
{
    try
    {
        if (0 == count) {
            return true;
        }
        const int first = row;
        const int last = row + count - 1;
        beginRemoveRows(parent, first, last);
        QMutexLocker locker(&_contactsMutex);
        if (count == _contacts.size()) {
            _contacts.clear();
        }
        else {
            _contacts.erase(_contacts.cbegin() + row, _contacts.cbegin() + row + count);
        }
    }
    catch (const std::exception&)
    {
        endRemoveRows();
        return false;
    }
    endRemoveRows();
    return true;
}

void ContactListModel::AddContacts(std::list<adbook::AdPersonDesc> && contacts)
{
    QMutexLocker locker(&_contactsMutex);
    int endPos = rowCount();
    insertRows(endPos, boost::numeric_cast<int>(contacts.size()));
    //_contacts.insert(_contacts.end(), contacts.cbegin(), contacts.cend());
    for (auto & item : contacts) {        
        _contacts.at(endPos++) = std::move(item);
    }
}

void ContactListModel::Clear()
{
    const int rc = rowCount();
    if (rc > 0) {
        removeRows(0, rc);
    }    
}

void ContactListModel::sort(int column, Qt::SortOrder order)
{
    emit layoutAboutToBeChanged();    
    QMutexLocker locker(&_contactsMutex);
    const ColumnData & colData = _columnsData.at(column);

    std::stable_sort(_contacts.begin(), _contacts.end(), 
        [&colData, order](const adbook::AdPersonDesc & l, const adbook::AdPersonDesc & r)
        {
            if (Qt::AscendingOrder == order) {
                return l.LexicographicalCompareStringAttrs(r, colData.attrId);
            }
            else {
                return r.LexicographicalCompareStringAttrs(l, colData.attrId);
            }            
        }
    );
    emit layoutChanged();
}

adbook::AdPersonDesc ContactListModel::GetContact(int row) const
{
    QMutexLocker locker(&_contactsMutex);
    return _contacts.at(boost::numeric_cast<size_t>(row));
}

void ContactListModel::SetContact(int r, const adbook::AdPersonDesc & apd)
{
    QMutexLocker locker(&_contactsMutex);
    _contacts.at(boost::numeric_cast<size_t>(r)) = apd;    
    QModelIndex topLeft = createIndex(r, 0), bottomRight = createIndex(r, columnCount() - 1);
    emit dataChanged(topLeft, bottomRight);
}

void ContactListModel::Save()
{    
    QMutexLocker locker(&_contactsMutex);
    _contacts.Save();    
}

void ContactListModel::Load()
{
    emit layoutAboutToBeChanged();
    QMutexLocker locker(&_contactsMutex);
    _contacts.Load();
    emit layoutChanged();
}