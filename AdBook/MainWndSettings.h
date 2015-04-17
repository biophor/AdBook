/*
Copyright (C) 2015 Goncharov Andrei.

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
#include "SearchFilters.h"

class MainWndSettings
{
public:    
    void Save();
    void Load();

    void SetRect(const CRect & rect) noexcept;
    const CRect & GetRect() const noexcept;

    using IntVector = std::vector<int>;
    
    template <class IntVectorType>
    void SetFilterColWidth(IntVectorType && fcw);
    
    template <class IntVectorType>
    void SetResultColWidth(IntVectorType && rcw);

    template <class IntVectorType>    
    void SetResultColOrder(IntVectorType && rci);

    const IntVector & GetFilterColWidth() const noexcept;
    const IntVector & GetResultColWidth() const noexcept;
    const IntVector & GetResultColOrder() const noexcept;
    
    using SearchFilterStrings = std::list<CString>;

    template <class SearchFilterStringsType>
    void SetSearchFilterStrings(SearchFilterStringsType && filterStrings);

    const SearchFilterStrings & GetSearchFilterStrings() const;

    enum class ConditionsCombineOperation {
        And, Or
    };
    void SetCondCombineOp(const ConditionsCombineOperation cco) {
        combineOperation_ = cco;
    }
    ConditionsCombineOperation GetCondCombineOp() const {
        return combineOperation_;
    }
    const SearchFilters & GetSearchFilters() const {
        return searchFilters_;
    }

    void SetSearchFilters(const SearchFilters & sf) {
        searchFilters_ = sf;
    }
private:
    SearchFilters searchFilters_;
    ConditionsCombineOperation combineOperation_ = ConditionsCombineOperation::And;
    CRect rect_;
    IntVector filterColWidth_;
    IntVector resultColWidth_;   // each column's width
    IntVector resultColOrder_;
    SearchFilterStrings searchFilterStrings_;
};

template <class IntVectorType>
void MainWndSettings::SetFilterColWidth(IntVectorType && fcw)
{
    static_assert(std::is_assignable<decltype(filterColWidth_), IntVectorType>::value, INVALID_PARAM_TYPE": fcw");
    filterColWidth_ = std::forward<IntVector>(fcw);
}

template <class IntVectorType>
void MainWndSettings::SetResultColWidth(IntVectorType && rcw)
{
    static_assert(std::is_assignable<decltype(resultColWidth_), IntVectorType>::value, INVALID_PARAM_TYPE": rcw");
    resultColWidth_ = std::forward<IntVector>(rcw);
}

template <class IntVectorType>
void MainWndSettings::SetResultColOrder(IntVectorType && rci)
{
    static_assert(std::is_assignable<decltype(resultColOrder_), IntVectorType>::value, INVALID_PARAM_TYPE": rci");
    resultColOrder_ = std::forward<IntVector>(rci);
}

template <class SearchFilterStringsType>
void MainWndSettings::SetSearchFilterStrings(SearchFilterStringsType && filterStrings)
{
    static_assert(std::is_assignable<decltype(searchFilterStrings_), SearchFilterStringsType>::value, INVALID_PARAM_TYPE": filterStrings");
    searchFilterStrings_ = std::forward<SearchFilterStringsType>(filterStrings);
}

