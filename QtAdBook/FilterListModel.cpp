// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
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

#include "stdafx.h"
#include "QSettingsAutoEndGroup.h"
#include "FilterListSettings.h"
#include "FilterListModel.h"


QList<FilterListModel::ColumnDef> FilterListModel::_columns = {
    { FilterNameColId, QObject::tr("Filter") },
    { ConditionColId, QObject::tr("Condition") },
    { ValueColId, QObject::tr("Value") }
};

FilterListModel::FilterListModel(
    AppSettings & appSettings,
    QObject * parent
)
    : QStandardItemModel(parent), _appSettings{ appSettings }
{
    auto colNames = GetColumnNames();
    setColumnCount(colNames.size());
    setHorizontalHeaderLabels(colNames);
}

int FilterListModel::FindFilter(FilterTypeItem * filterTypeItem, FilterConditionItem * conditionItem,
    const QString & filterValue)
{
    QList<QStandardItem*> sameItems = findItems(filterTypeItem->text(), Qt::MatchExactly);
    for (auto it : sameItems) {
        int r = it->row();
        bool typeEqual = static_cast<FilterTypeItem*>(it)->GetFilterCode() ==
            filterTypeItem->GetFilterCode();
        bool conditionEqual = static_cast<FilterConditionItem*>(item(r, ConditionColId))->GetMatchingRule() ==
            conditionItem->GetMatchingRule();
        bool valueEqual = item(r, ValueColId)->text() == filterValue;
        if (typeEqual && conditionEqual && valueEqual) {
            return r;
        }
    }
    return -1;
}

int FilterListModel::AddFilter(FilterTypeItem * filterTypeItem, FilterConditionItem * conditionItem,
    const QString & filterValue)
{
    int r = FindFilter(filterTypeItem, conditionItem, filterValue);
    if (r != -1) {
        return r;
    }
    QList<QStandardItem*> items{ filterTypeItem, conditionItem, new QStandardItem(filterValue) };
    appendRow(items);
    return rowCount() - 1;
}

std::wstring FilterListModel::ConstructLdapRequest(bool AllConditionsShouldBeMet)
{
    adbook::LdapRequestBuilder lr;
    const int itemCount = rowCount();
    for (int i = 0; i < itemCount; ++i) {
        FilterTypeItem * typeItem = GetFilterType(i);
        FilterConditionItem * condition = GetFilterCondition(i);
        QString filterValue = GetFilterValue(i);
        if (typeItem->GetFilterType() == FilterType::Composite) {
            if (typeItem->GetFilterCode() == FilterCode(CompositeFilterId::AnyAttribute)) {
                auto & attributes = adbook::Attributes::GetInstance();
                const auto attrIds = attributes.GetAttrIds();
                for (const auto & ii : attrIds) {
                    if (attributes.IsString(ii)) {
                        lr.AddRule(ii, condition->GetMatchingRule(), filterValue.toStdWString());
                    }
                }
                lr.AddOR();
            }
        }
    }
    for (int i = 0; i < itemCount; ++i) {
        FilterTypeItem * typeItem = GetFilterType(i);
        FilterConditionItem * condition = GetFilterCondition(i);
        QString filterValue = GetFilterValue(i);
        if (typeItem->GetFilterType() == FilterType::LdapAttr) {
            lr.AddRule(std::get<adbook::Attributes::AttrId>(typeItem->GetFilterCode()),
                condition->GetMatchingRule(), filterValue.toStdWString());
        }
    }
    if (itemCount != 0) {
        if (AllConditionsShouldBeMet) {
            lr.AddAND();
        }
        else {
            lr.AddOR();
        }
    }
    lr.AddRule(L"objectCategory", adbook::LdapRequestBuilder::ExactMatch, L"person");
    if (itemCount != 0) {
        lr.AddAND();
    }
    return lr.Get();
}

QStringList FilterListModel::GetFilterValues()
{
    QStringList filterValues;
    const int itemCount = rowCount();
    for (int i = 0; i < itemCount; ++i) {
        filterValues.push_back(GetFilterValue(i));
    }
    return filterValues;
}

namespace SearchFilterSettings
{
const QString sectionName = "SearchFilter";
const QString typeParam = "Type";
const QString numFiltersParam = "numFilters";
const QString codeParam = "Code";
const QString conditionParam = "Condition";
const QString valueParam = "Value";
}


void FilterListModel::SaveState()
{
    FilterListSettings fls;
    for (int i = 0, count = rowCount(); i < count; ++i) {
        FilterTypeItem * type = GetFilterType(i);
        FilterConditionItem * cond = GetFilterCondition(i);
        fls.AddFilter({ type->GetFilterCode(), GetFilterValue(i), cond->GetMatchingRule() });
    }
    _appSettings.SetFilterListSettings(fls);
}

void FilterListModel::LoadState
(
    FilterCode filterCode,
    const QString & filterValue,
    FilterCondition condition
)
{
    if (filterValue.trimmed().isEmpty()) {
        throw adbook::HrError(E_INVALIDARG, L"filterValue is empty", __FUNCTIONW__);
    }
    FilterTypeItem * typeItem = nullptr;
    if (auto compositeFilterIdPtr = std::get_if<CompositeFilterId>(&filterCode)) {
        if (CompositeFilterId::AnyAttribute == *compositeFilterIdPtr) {
            typeItem = new FilterTypeItem(CompositeFilterId::AnyAttribute);
        }
        else {
            throw adbook::HrError(E_INVALIDARG, L"unknown CompositeFilterId", __FUNCTIONW__);
        }
    }
    else  if (auto attrIdPtr = std::get_if<adbook::Attributes::AttrId>(&filterCode)) {
        typeItem = new FilterTypeItem(*attrIdPtr);
    }
    else {
        throw adbook::HrError(E_INVALIDARG, L"unknown filterCode", __FUNCTIONW__);
    }
    FilterConditionItem * condItem = new FilterConditionItem(condition);
    AddFilter(typeItem, condItem, filterValue);

}

void FilterListModel::LoadState()
{
    removeRows(0, rowCount());
    FilterListSettings fls = _appSettings.GetFilterListSettings();
    for (size_t i = 0; i < fls.GetNumFilters(); ++i) {
        auto filter = fls.GetFilter(i);
        LoadState(filter.GetFilterCode(), filter.GetValue(), filter.GetCondition());
    }
}

