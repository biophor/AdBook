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

class SearchFilterTypesModelTests : public ::testing::Test {
public:
    void SetUp() override {

    }
    void TearDown() override {

    }
};

TEST_F(SearchFilterTypesModelTests, Model_provides_data_for_QT)
{
    // Arrange
    SearchFilterTypesModel sut;
    const int numFilters = sut.rowCount();
    QHash<int, QByteArray> roles = sut.roleNames();

    // Act & Assert
    for (int rowIndex = 0; rowIndex < numFilters; ++rowIndex) {

        for (int role : roles.keys()) {
            QVariant v = sut.data(sut.index(rowIndex), role);
            ASSERT_TRUE(v.type() == QVariant::String);

            QString filterDisplayName = v.toString();
            ASSERT_FALSE(filterDisplayName.isEmpty());
        }
    }
}

TEST_F(SearchFilterTypesModelTests, Model_provides_data_by_row_index)
{
    // Arrange
    qmladbook::SearchFilterTypesModel sut;
    const int numFilters = sut.rowCount();

    // Act & Assert
    for (int rowIndex = 0; rowIndex < numFilters; ++rowIndex) {

        const qmladbook::SearchFilterType type = sut.Get(rowIndex);
        const QString displayName = type.GetUiName();
        const qmladbook::SearchFilterTypeId id = type.GetId();

        ASSERT_TRUE(id != qmladbook::SearchFilterTypeId::Invalid);
        ASSERT_FALSE(displayName.isEmpty());
    }
}

TEST_F(SearchFilterTypesModelTests, Model_is_not_empty)
{
    qmladbook::SearchFilterTypesModel sut;

    ASSERT_TRUE(sut.rowCount() > 0);
}

TEST_F(SearchFilterTypesModelTests, Model_contains_different_filter_types)
{
    // Arrange
    qmladbook::SearchFilterTypesModel sut;
    std::set<QString> uniqueDisplayNames;
    std::set< qmladbook::SearchFilterTypeId> uniqueFiltersIds;
    int numFilters = sut.rowCount();

    // Act
    for (int i = 0; i < numFilters; ++i) {
        const qmladbook::SearchFilterType type = sut.Get(i);
        uniqueDisplayNames.insert(type.GetUiName());
        uniqueFiltersIds.insert(type.GetId());
    }

    // Assert
    ASSERT_TRUE(uniqueDisplayNames.size() == uniqueFiltersIds.size());
    ASSERT_TRUE(uniqueFiltersIds.size() == numFilters);
}

