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
#include "SearchFilterType.h"
#include "SearchFilterTypesModel.h"

namespace qmladbook
{
    std::once_flag SearchFilterTypesModel::initializationCompletted;
    QVector<SearchFilterType> SearchFilterTypesModel::filterTypes;

    SearchFilterTypesModel::SearchFilterTypesModel(QObject *parent)
        : QAbstractListModel(parent)
    {
        std::call_once(initializationCompletted, OneTimeInitialization);

    }

    void SearchFilterTypesModel::AddFilterType(SearchFilterTypeId id, const QString & uiName)
    {
        SearchFilterType filterType{ id, uiName };
        filterTypes.append(filterType);
    }

    void SearchFilterTypesModel::AddFilterType(SearchFilterTypeId id, adbook::Attributes::AttrId attrId)
    {
        auto & attrTraits = adbook::Attributes::GetInstance();
        SearchFilterType filterType{ id, QString::fromStdWString(attrTraits.GetUiAttrName(attrId)) };
        filterTypes.append(filterType);
    }

    SearchFilterType SearchFilterTypesModel::Get(int row) const
    {
        if (row < 0 || row >= filterTypes.count()) {
            throw adbook::HrError(E_INVALIDARG, L"row", __FUNCTIONW__);
        }
        return filterTypes[row];
    }

    int SearchFilterTypesModel::rowCount(const QModelIndex & parent) const
    {
        Q_UNUSED(parent);
        return filterTypes.size();
    }

    QVariant SearchFilterTypesModel::data(const QModelIndex & index, int role) const
    {
        if (!index.isValid()) {
            return QVariant();
        }
        if (index.row() < 0 || index.row() >= filterTypes.size()) {
            return QVariant();
        }
        return (role == Qt::DisplayRole) ? filterTypes.at(index.row()).GetUiName() : QVariant();
    }

    QHash<int, QByteArray> SearchFilterTypesModel::roleNames() const
    {
        return { {Qt::DisplayRole, "display"} };
    }

    SearchFilterType SearchFilterTypesModel::GetTypeById(SearchFilterTypeId id)
    {
        for (SearchFilterType type : filterTypes) {
            if (type.GetId() == id) {
                return type;
            }
        }
        throw adbook::HrError(E_INVALIDARG, L"id", __FUNCTIONW__);
    }

    void SearchFilterTypesModel::OneTimeInitialization()
    {
        AddFilterType(SearchFilterTypeId::AnyTextAttribute, tr("Any attribute"));
        AddFilterType(SearchFilterTypeId::Names, tr("Names: Common Name, Surname, Given name, Display name"));
        AddFilterType(SearchFilterTypeId::Phones, tr("Phones: Home phone, Mobile phone, Work phone"));
        AddFilterType(SearchFilterTypeId::CommonName, adbook::Attributes::CommonName);
        AddFilterType(SearchFilterTypeId::Email, adbook::Attributes::Email);
        AddFilterType(SearchFilterTypeId::WorkPhone, adbook::Attributes::WorkPhone);
        AddFilterType(SearchFilterTypeId::Company, adbook::Attributes::Company);
        AddFilterType(SearchFilterTypeId::Title, adbook::Attributes::Title);
        AddFilterType(SearchFilterTypeId::Department, adbook::Attributes::Department);
        AddFilterType(SearchFilterTypeId::EmployeeID, adbook::Attributes::EmpId);
        AddFilterType(SearchFilterTypeId::SurName, adbook::Attributes::SurName);
        AddFilterType(SearchFilterTypeId::GivenName, adbook::Attributes::GivenName);
        AddFilterType(SearchFilterTypeId::DisplayName, adbook::Attributes::DisplayName);
        AddFilterType(SearchFilterTypeId::HomePhone, adbook::Attributes::HomePhone);
        AddFilterType(SearchFilterTypeId::MobilePhone, adbook::Attributes::MobilePhone);
        AddFilterType(SearchFilterTypeId::StateOrProvince, adbook::Attributes::State);
        AddFilterType(SearchFilterTypeId::LocalityOrCity, adbook::Attributes::Locality);
    }
}
