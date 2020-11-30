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
#include "SearchFilterTypesModel.h"
#include "SearchFilterRulesModel.h"
#include "SearchFiltersModel.h"

namespace qmladbook
{
    SearchFiltersModel::SearchFiltersModel(QObject *parent)
        : QAbstractListModel(parent)
    {
    }

    QVariant SearchFiltersModel::data(const QModelIndex & index, int nRole) const
    {
        if (!index.isValid()) {
            return {};
        }
        if (index.row() < 0 || index.row() >= _filters.count()) {
            return {};
        }
        switch (nRole) {
        case FilterTypeRole:
            return _filters.at(index.row()).GetType().GetUiName();

        case FilterRuleRole:
            return _filters.at(index.row()).GetRule().GetUiName();

        case FilterValueRole:
            return _filters.at(index.row()).GetValue();

        default:
            return {};
        }
    }

    int SearchFiltersModel::rowCount(const QModelIndex & parent) const
    {
        if (parent.isValid()) {
            throw adbook::HrError(E_INVALIDARG, L"parent", __FUNCTIONW__);
        }
        return _filters.count();
    }

    QVariant SearchFiltersModel::headerData(int nSection, Qt::Orientation orientation, int nRole) const
    {
        Q_UNUSED(nSection);
        if (orientation != Qt::Horizontal) {
            return {};
        }
        switch (nRole) {
        case FilterTypeRole:
            return tr("Filter");
        case FilterRuleRole:
            return tr("Matching rule");
        case FilterValueRole:
            return tr("Value");
        default:
            return {};
        }
    }

    Qt::ItemFlags SearchFiltersModel::flags(const QModelIndex &index) const
    {
        Qt::ItemFlags flags = QAbstractListModel::flags(index);
        if (index.column() == 2) {
            flags |= Qt::ItemIsEditable;
        }
        return flags;
    }

    bool SearchFiltersModel::insertRows(int nRow, int nCount, const QModelIndex & parent)
    {
        if (parent.isValid()) {
            return false;
        }
        if (nRow < 0 || nRow > rowCount()) {
            return false;
        }
        beginInsertRows(QModelIndex(), nRow, nRow + nCount - 1);
        try {
            for (int i = 0; i < nCount; ++i) {
                _filters.insert(nRow, {});
            }
        }
        catch (const std::exception & e) {
            endInsertRows();
            MY_TRACE_STDEXCEPTION(e);
            return false;
        }
        endInsertRows();
        return true;
    }

    bool SearchFiltersModel::removeRows(int nRow, int nCount, const QModelIndex & parent)
    {
        if (parent.isValid()) {
            return false;
        }
        if (nRow < 0 || nRow >= rowCount()) {
            return false;
        }
        beginRemoveRows(QModelIndex(), nRow, nRow + nCount - 1);
        try {
            for (int i = 0; i < nCount; ++i) {
                _filters.removeAt(nRow);
            }
        }
        catch (const std::exception & e) {
            endRemoveRows();
            MY_TRACE_STDEXCEPTION(e);
            return false;
        }
        endRemoveRows();
        return true;
    }

    QHash<int, QByteArray> SearchFiltersModel::roleNames() const
    {
        QHash<int, QByteArray> roles{
            { FilterTypeRole , "FilterType" },
            { FilterRuleRole, "FilterRule" },
            { FilterValueRole, "FilterValue" }
        };
        return roles;
    }

    bool SearchFiltersModel::Exists(const SearchFilter & searchFilter)
    {
        return std::find(_filters.cbegin(), _filters.cend(), searchFilter) != _filters.cend();
    }

    void SearchFiltersModel::Add(const SearchFilter & searchFilter)
    {
        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        try {
            _filters.append(searchFilter);
        }
        catch (const std::exception & e) {
            endInsertRows();
            MY_TRACE_STDEXCEPTION(e);
            throw;
        }
        endInsertRows();
    }

    void SearchFiltersModel::Remove(int row)
    {
        if (row < 0 || row >= _filters.count()) {
            throw adbook::HrError(E_INVALIDARG, L"row", __FUNCTIONW__);
        }
        beginRemoveRows(QModelIndex(), row, row);
        try {
            _filters.removeAt(row);
        }
        catch (const std::exception & e) {
            endRemoveRows();
            MY_TRACE_STDEXCEPTION(e);
            throw;
        }
        endRemoveRows();
    }

    const SearchFilter & SearchFiltersModel::Get(int row) const
    {
        if (row < 0 || row >= _filters.count()) {
            throw adbook::HrError(E_INVALIDARG, L"row", __FUNCTIONW__);
        }
        return _filters[row];
    }

    void SearchFiltersModel::Set(int row, const SearchFilter & searchFilter)
    {
        if (row < 0 || row >= _filters.count()) {
            throw adbook::HrError(E_INVALIDARG, L"row", __FUNCTIONW__);
        }
        _filters[row] = searchFilter;
        emit dataChanged(index(row), index(row));
    }

    QVector<SearchFilter> SearchFiltersModel::GetFilters() const {
        return _filters;
    }

    void SearchFiltersModel::SetFilters(const QVector<SearchFilter> & filters) {
        _filters = filters;
    }

    QDataStream & operator << (QDataStream & strm, SearchFilterTypeId searchFilterTypeId)
    {
        strm << static_cast<int>(searchFilterTypeId);
        return strm;
    }

    QDataStream & operator << (QDataStream & strm, SearchFilterRuleId searchFilterRuleId)
    {
        strm << static_cast<int>(searchFilterRuleId);
        return strm;
    }

    QDataStream & operator >> (QDataStream & strm, SearchFilterTypeId & searchFilterTypeId)
    {
        int tmp{};
        strm >> tmp;
        searchFilterTypeId = static_cast<SearchFilterTypeId>(tmp);
        return strm;
    }

    QDataStream & operator >> (QDataStream & strm, SearchFilterRuleId & searchFilterRuleId)
    {
        int tmp{};
        strm >> tmp;
        searchFilterRuleId = static_cast<SearchFilterRuleId>(tmp);
        return strm;
    }

    QByteArray SearchFiltersModel::GetSerializedFilters() const
    {
        QByteArray searchFiltersBytes;
        QBuffer dataStreamBuffer(&searchFiltersBytes);
        dataStreamBuffer.open(QIODevice::WriteOnly);
        QDataStream dataStream(&dataStreamBuffer);
        dataStream.setVersion(QDataStream::Qt_5_12);

        dataStream << static_cast<quint32>(_serializedFiltersVersion);
        dataStream << static_cast<qint32>(_filters.size());

        for (auto && filter : _filters) {
            dataStream << filter.GetType().GetId() << filter.GetRule().GetId() << filter.GetValue();
        }
        return searchFiltersBytes;
    }

    void SearchFiltersModel::SetFilters(const QByteArray & serializedFilters)
    {
        if (serializedFilters.isEmpty()) {
            return;
        }
        QByteArray writeableSerializedFilters = serializedFilters;
        QBuffer dataStreamBuffer(&writeableSerializedFilters);
        dataStreamBuffer.open(QIODevice::ReadOnly);
        QDataStream dataStream(&dataStreamBuffer);
        dataStream.setVersion(QDataStream::Qt_5_12);

        quint32 serializedFiltersVersion{0};
        dataStream >> serializedFiltersVersion;
        if (serializedFiltersVersion != _serializedFiltersVersion) {
            return;
        }
        qint32 numFilters = 0;
        dataStream >> numFilters;

        for (qint32 i = 0; i < numFilters; ++i)
        {
            SearchFilterTypeId typeId;
            SearchFilterRuleId ruleId;
            QString value;
            dataStream >> typeId >> ruleId >> value;
            SearchFilterType type = SearchFilterTypesModel::GetTypeById(typeId);
            SearchFilterRule rule = SearchFilterRulesModel::GetRuleById(ruleId);
            _filters.append({type, rule, value});
        }
    }
}
