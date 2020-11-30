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
#include "SearchFilterRulesModel.h"

namespace qmladbook
{
    std::once_flag SearchFilterRulesModel::initializationCompletted;
    QVector<SearchFilterRule> SearchFilterRulesModel::filterRules;

    SearchFilterRulesModel::SearchFilterRulesModel(QObject *parent)
        : QAbstractListModel(parent)
    {
        std::call_once(initializationCompletted, OneTimeInitialization);
    }

    SearchFilterRule SearchFilterRulesModel::Get(int row) const
    {
        if (row < 0 || row >= filterRules.count()) {
            throw adbook::HrError(E_INVALIDARG, L"row", __FUNCTIONW__);
        }
        return filterRules.at(row);
    }

    QHash<int, QByteArray> SearchFilterRulesModel::roleNames() const
    {
        return { {Qt::DisplayRole, "display"} };
    }

    QVariant SearchFilterRulesModel::data(const QModelIndex &index, int role) const
    {
        if (!index.isValid()) {
            return {};
        }
        if (index.row() < 0 || index.row() >= filterRules.count()) {
            return {};
        }
        return (role == Qt::DisplayRole) ? filterRules.at(index.row()).GetUiName() : QVariant{};
    }

    int SearchFilterRulesModel::rowCount(const QModelIndex &parent) const
    {
        if (parent.isValid()) {
            throw adbook::HrError(E_INVALIDARG, L"parent", __FUNCTIONW__);
        }
        return filterRules.count();
    }

    void SearchFilterRulesModel::OneTimeInitialization()
    {
        filterRules.append(SearchFilterRule(SearchFilterRuleId::Contains, tr("contains")));
        filterRules.append(SearchFilterRule(SearchFilterRuleId::ExactMatch, tr("exact match")));
        filterRules.append(SearchFilterRule(SearchFilterRuleId::BeginsWith, tr("begins with")));
        filterRules.append(SearchFilterRule(SearchFilterRuleId::EndsWith, tr("ends with")));
    }

    SearchFilterRule SearchFilterRulesModel::GetRuleById(SearchFilterRuleId id)
    {
        for (SearchFilterRule rule : filterRules) {
            if (rule.GetId() == id) {
                return rule;
            }
        }
        throw adbook::HrError(E_INVALIDARG, L"id", __FUNCTIONW__);
    }
}

