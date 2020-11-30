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
#include "LdapRequestCreator.h"

namespace qmladbook
{
    namespace {
        struct FilterData {
            adbook::Attributes::AttrId id;
            adbook::LdapRequestBuilder::MatchingRule mr;
            std::wstring value;
            bool operator < (const FilterData & that) const
            {
                if (id == that.id) {
                    if (mr == that.mr) {
                        if (value == that.value) {
                            return false;
                        }
                        else {
                            return value < that.value;
                        }
                    }
                    else {
                        return mr < that.mr;
                    }
                }
                else {
                    return id < that.id;
                }
            }
        };
    }

    LdapRequestCreator::LdapRequestCreator(const QVector<SearchFilter> & filters, bool allRulesAreMandatory)
    {
        std::set<FilterData> filterDataSet;
        const int numRules = filters.size();

        for (int i = 0; i < numRules; ++i)
        {
            const SearchFilter & filter = filters.at(i);
            const SearchFilterType type = filter.GetType();
            const SearchFilterRule rule = filter.GetRule();
            const QString value = filter.GetValue();
            const SearchFilterTypeId filterId = type.GetId();
            const SearchFilterRuleId ruleId = rule.GetId();
            const std::vector<adbook::Attributes::AttrId> attrIds = FilterIdToAttrIds(filterId);
            const adbook::LdapRequestBuilder::MatchingRule matchingRule = FilterRuleToLdapMatchingRule(ruleId);

            for (auto attrIdIter = attrIds.cbegin(); attrIdIter != attrIds.cend(); ++attrIdIter)
            {
                FilterData filterData{ *attrIdIter, matchingRule, value.toStdWString() };
                filterDataSet.insert(filterData);
            }
        }
        adbook::LdapRequestBuilder ldapRequest;
        for (auto[attrId, matchingRule, value] : filterDataSet) {
            ldapRequest.AddRule(attrId, matchingRule, value);
        }
        if (numRules > 0) {
            if (allRulesAreMandatory) {
                ldapRequest.AddAND();
            }
            else {
                ldapRequest.AddOR();
            }
        }
        ldapRequest.AddRule(L"objectCategory", adbook::LdapRequestBuilder::ExactMatch, L"person");
        if (numRules > 0) {
            ldapRequest.AddAND();
        }
        _ldapRequestText = QString::fromStdWString(ldapRequest.Get());
    }

    std::vector<adbook::Attributes::AttrId>
        LdapRequestCreator::FilterIdToAttrIds(SearchFilterTypeId filterId)
    {
        std::vector<adbook::Attributes::AttrId> ret;

        switch (filterId)
        {
        case SearchFilterTypeId::AnyTextAttribute:
        {
            auto & attrTraits = adbook::Attributes::GetInstance();
            for (auto attrId : attrTraits.GetAttrIds()) {
                if (attrTraits.IsEditableString(attrId)) {
                    ret.push_back(attrId);
                }
            }
        }
        break;

        case SearchFilterTypeId::CommonName:
            ret.push_back(adbook::Attributes::CommonName);
            break;

        case SearchFilterTypeId::Company:
            ret.push_back(adbook::Attributes::Company);
            break;

        case SearchFilterTypeId::Department:
            ret.push_back(adbook::Attributes::Department);
            break;

        case SearchFilterTypeId::DisplayName:
            ret.push_back(adbook::Attributes::DisplayName);
            break;

        case SearchFilterTypeId::Email:
            ret.push_back(adbook::Attributes::Email);
            break;

        case SearchFilterTypeId::EmployeeID:
            ret.push_back(adbook::Attributes::EmpId);
            break;

        case SearchFilterTypeId::GivenName:
            ret.push_back(adbook::Attributes::GivenName);
            break;

        case SearchFilterTypeId::HomePhone:
            ret.push_back(adbook::Attributes::HomePhone);
            break;

        case SearchFilterTypeId::LocalityOrCity:
            ret.push_back(adbook::Attributes::Locality);
            break;

        case SearchFilterTypeId::MobilePhone:
            ret.push_back(adbook::Attributes::MobilePhone);
            break;

        case SearchFilterTypeId::Names:
            ret.push_back(adbook::Attributes::CommonName);
            ret.push_back(adbook::Attributes::DisplayName);
            ret.push_back(adbook::Attributes::GivenName);
            ret.push_back(adbook::Attributes::SurName);
            break;

        case SearchFilterTypeId::Phones:
            ret.push_back(adbook::Attributes::HomePhone);
            ret.push_back(adbook::Attributes::MobilePhone);
            ret.push_back(adbook::Attributes::WorkPhone);
            break;

        case SearchFilterTypeId::StateOrProvince:
            ret.push_back(adbook::Attributes::State);
            break;

        case SearchFilterTypeId::SurName:
            ret.push_back(adbook::Attributes::SurName);
            break;

        case SearchFilterTypeId::Title:
            ret.push_back(adbook::Attributes::Title);
            break;

        case SearchFilterTypeId::WorkPhone:
            ret.push_back(adbook::Attributes::WorkPhone);
            break;

        default:
            throw adbook::HrError(E_INVALIDARG, L"filterId", __FUNCTIONW__);
        }
        return ret;
    }

    adbook::LdapRequestBuilder::MatchingRule
        LdapRequestCreator::FilterRuleToLdapMatchingRule(SearchFilterRuleId ruleId)
    {
        switch (ruleId) {
        case SearchFilterRuleId::BeginsWith:
            return adbook::LdapRequestBuilder::BeginWith;

        case SearchFilterRuleId::Contains:
            return adbook::LdapRequestBuilder::Contains;

        case SearchFilterRuleId::EndsWith:
            return adbook::LdapRequestBuilder::EndWith;

        case SearchFilterRuleId::ExactMatch:
            return adbook::LdapRequestBuilder::ExactMatch;
        }
        return adbook::LdapRequestBuilder::InvalidMatchingRule;
    }

    QString LdapRequestCreator::Get()
    {
        return _ldapRequestText;
    }
}
