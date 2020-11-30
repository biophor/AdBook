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
#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "../AdBookBL/export.h"
#include "../QmlAdBookBL/export.h"

using namespace qmladbook;

class SearchFiltersModelTests : public ::testing::Test {
public:
    void SetUp() override {

    }
    void TearDown() override {

    }
};

SearchFilter CreateSearchFilterForTesting()
{
    SearchFilterTypesModel sftm;
    SearchFilterRulesModel sfrm;

    SearchFilterType type = sftm.Get(0);
    SearchFilterRule rule = sfrm.Get(0);
    SearchFilter sf(type, rule, "value");
    return sf;
}

std::vector<SearchFilter> CreateSearchFiltersForTesting(size_t numFilters = 8)
{
    SearchFilterTypesModel sftm;
    SearchFilterRulesModel sfrm;
    std::vector<SearchFilter> searchFilters;

    for (size_t i = 0; i < numFilters; ++i) {

        SearchFilterType type = sftm.Get(0);
        SearchFilterRule rule = sfrm.Get(0);
        SearchFilter searchFilter(type, rule, QString("value %1").arg(i));
        searchFilters.push_back(searchFilter);
    }
    return searchFilters;
}

TEST_F(SearchFiltersModelTests, Changing_an_existing_filter)
{
    // Arrange
    SearchFiltersModel sut;
    SearchFilter searchFilter = CreateSearchFilterForTesting();
    sut.Add(searchFilter);

    // Act
    searchFilter.SetValue("new value");
    sut.Set(0, searchFilter);

    // Assert
    ASSERT_TRUE(sut.Get(0).GetValue() == "new value");
}

TEST_F(SearchFiltersModelTests, Adding_a_filter)
{
    // Arrange
    SearchFiltersModel sut;
    SearchFilter searchFilter = CreateSearchFilterForTesting();

    // Act
    sut.Add(searchFilter);

    // Assert
    ASSERT_TRUE(sut.Get(0) == searchFilter);
}

TEST_F(SearchFiltersModelTests, Removing_a_filter)
{
    // Arrange
    SearchFiltersModel sut;
    std::vector<SearchFilter> searchFilters = CreateSearchFiltersForTesting();
    for (const auto & searchFilter : searchFilters) {
        sut.Add(searchFilter);
    }

    // Act
    int rowCountBefore = sut.rowCount();
    sut.Remove(0);
    int rowCountAfter = sut.rowCount();

    // Assert
    ASSERT_TRUE(rowCountBefore == rowCountAfter + 1);
}

TEST_F(SearchFiltersModelTests, Checking_existing_filter)
{
    // Arrange
    SearchFiltersModel sut;
    std::vector<SearchFilter> searchFilters = CreateSearchFiltersForTesting();
    for (const auto & searchFilter : searchFilters) {
        sut.Add(searchFilter);
    }

    // Act
    bool exists = sut.Exists(searchFilters.back());

    // Assert
    ASSERT_TRUE(exists);
}

TEST_F(SearchFiltersModelTests, Model_provides_data_for_QT)
{
    // Arrange
    SearchFiltersModel sut;
    SearchFilter searchFilter = CreateSearchFilterForTesting();
    sut.Add(searchFilter);

    // Act & Assert
    for (int role : sut.roleNames().keys()) {
        QVariant v = sut.data(sut.index(0), role);
        ASSERT_TRUE(v.type() == QVariant::String);

        QString s = v.toString();
        ASSERT_FALSE(s.isEmpty());
    }
}

TEST_F(SearchFiltersModelTests, Model_provides_header_information_for_QT)
{
    // Arrange
    SearchFiltersModel sut;

    // Act & Assert
    for (int role : sut.roleNames().keys())
    {
        QVariant v = sut.headerData(0, Qt::Horizontal, role);
        ASSERT_TRUE(v.type() == QVariant::String);

        QString s = v.toString();
        ASSERT_FALSE(s.isEmpty());
    }
}

TEST_F(SearchFiltersModelTests, Model_provides_data_by_row_index)
{
    // Arrange
    SearchFiltersModel sut;
    std::vector<SearchFilter> searchFilters = CreateSearchFiltersForTesting();
    for (const auto & searchFilter : searchFilters) {
        sut.Add(searchFilter);
    }

    // Act & Assert
    for (int i = 0; i < sut.rowCount(); ++i) {
        ASSERT_TRUE(sut.Get(i) == searchFilters.at(i));
    }
}

