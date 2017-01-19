// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
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
#include "FilterListModel.h"


QList<FilterListModel::ColumnDef> FilterListModel::_columns = {
    { FilterNameColId, QObject::tr("Filter") },
    { ConditionColId, QObject::tr("Condition") },
    { ValueColId, QObject::tr("Value") }
};


FilterListModel::FilterListModel(QObject * parent) : QStandardItemModel(parent) {
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
    /*insertRow(rowCount());
    r = rowCount() - 1;
    setItem(r, FilterNameColId, filterTypeItem);
    setItem(r, ConditionColId, conditionItem);
    setItem(r, ValueColId, new QStandardItem(filterValue));    
    return r;*/
}

adbook::LdapRequest FilterListModel::ConstructLdapRequest(bool AllConditionsShouldBeMet)
{
    adbook::LdapRequest lr;
    const int itemCount = rowCount();
    for (int i = 0; i < itemCount; ++i) {
        FilterTypeItem * typeItem = GetFilterType(i);
        FilterConditionItem * condition = GetFilterCondition(i);
        QString filterValue = GetFilterValue(i);
        if (typeItem->GetFilterType() == FilterType::Composite) {
            if (typeItem->GetFilterCode() == static_cast<int>(CompositeFilterId::AnyAttribute)) {
                const auto attrIds = adbook::Attributes::GetInstance().GetAttrIds();
                for (const auto & ii : attrIds) {
                    if (adbook::Attributes::GetInstance().IsString(ii)) {
                        lr.AddRule(ii, condition->GetMatchingRule(), filterValue.toStdWString());
                    }
                }
                lr.AddOR();
            }
        }
        else if (typeItem->GetFilterType() == FilterType::LdapAttr) {
            lr.AddRule(static_cast<adbook::Attributes::AttrId>(typeItem->GetFilterCode()),
                condition->GetMatchingRule(), filterValue.toStdWString());
        }
        else {

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
    lr.AddRule(L"objectCategory", adbook::LdapRequest::ExactMatch, L"person");
    if (itemCount != 0) {
        lr.AddAND();
    }
    return lr;
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

void FilterListModel::Load(int filterType, int filterCode, const QString & filterValue, int condition)
{
    if (filterValue.trimmed().isEmpty()) {
        throw adbook::HrError(E_INVALIDARG, L"filterValue", __FUNCTIONW__);
    }
    FilterTypeItem * typeItem = nullptr;
    
    FilterType filterType_ = static_cast<FilterType>(filterType);
    adbook::LdapRequest::MathingRule condition_ = static_cast<adbook::LdapRequest::MathingRule>(condition);
    
    if (FilterType::Composite == filterType_) {
        CompositeFilterId compositeFilterId = static_cast<CompositeFilterId>(filterCode);
        if (CompositeFilterId::AnyAttribute == compositeFilterId) {
            typeItem = new FilterTypeItem(compositeFilterId);
        }
        else {
            throw adbook::HrError(E_INVALIDARG, L"filterCode", __FUNCTIONW__);
        }
    }
    else if (FilterType::LdapAttr == filterType_) {
        adbook::Attributes::AttrId attrId = static_cast<adbook::Attributes::AttrId>(filterCode);        
        typeItem = new FilterTypeItem(attrId);
    }
    else {
        throw adbook::HrError(E_INVALIDARG, L"filterType", __FUNCTIONW__);
    }
    FilterConditionItem * condItem = new FilterConditionItem(condition_);
    AddFilter(typeItem, condItem, filterValue);
}

void FilterListModel::Load()
{
    removeRows(0, rowCount());
    using namespace SearchFilterSettings;
    QSettings settings(QApplication::organizationName(), QApplication::applicationName());
    settings.beginGroup(sectionName);
        int numFilters = settings.value(numFiltersParam).toInt();
        for (int i = 0; i < numFilters; ++i) {
            settings.beginGroup(QString::number(i));
                int filterCode = settings.value(codeParam).toInt();
                QString filterVal = settings.value(valueParam).toString();
                int filterType = settings.value(typeParam).toInt();
                int filterCond = settings.value(conditionParam).toInt();
                Load(filterType, filterCode, filterVal, filterCond);
            settings.endGroup();
        }         
    settings.endGroup();    
}

void FilterListModel::Save()
{    
    using namespace SearchFilterSettings;
    QSettings settings(QApplication::organizationName(), QApplication::applicationName());
    
    settings.beginGroup(sectionName);        
        settings.setValue(numFiltersParam, rowCount());
        for (int i = 0, count = rowCount(); i < count; ++i) {
            settings.beginGroup(QString::number(i));
                FilterTypeItem * type = GetFilterType(i);
                settings.setValue(typeParam, static_cast<int>(type->GetFilterType()));
                settings.setValue(codeParam, static_cast<int>(type->GetFilterCode()));
                FilterConditionItem * cond = GetFilterCondition(i);
                settings.setValue(conditionParam, static_cast<int>(cond->GetMatchingRule()));
                QString val = GetFilterValue(i);
                settings.setValue(valueParam, val);
            settings.endGroup();
        }
    settings.endGroup();
}

