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
#include <QGuiApplication>
#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "../AdBookBL/export.h"
#include "../QmlAdBookBL/export.h"

#include "TestSuiteBase.h"

using namespace qmladbook;
using namespace testing;

class MainWindowViewModelTests : public TestSuiteBase {
public:
    void SetUp() override {
    }
    void TearDown() override {
    }
};

TEST_F(MainWindowViewModelTests, Can_add_a_new_search_filter)
{
    // Arrange
    MainWindowViewModel sut;

    int searchFilterTypeIndex = 0;
    int searchFilterRuleIndex = 0;
    QString filterValue = "Stanley";

    // Act
    bool filterAdded = sut.addNewFilter(searchFilterTypeIndex, searchFilterRuleIndex, filterValue);

    // Assert
    ASSERT_TRUE(filterAdded);

    auto filtersModel = sut.searchFiltersModel();
    ASSERT_TRUE(filtersModel->rowCount() == 1);

    SearchFilterType filterType = sut.searchFilterTypesModel()->Get(searchFilterTypeIndex);
    SearchFilterRule filterRule = sut.searchFilterRulesModel()->Get(searchFilterRuleIndex);

    SearchFilter searchFilter = filtersModel->Get(0); // get the filter just added

    ASSERT_TRUE(searchFilter.GetValue() == filterValue);
    ASSERT_TRUE(searchFilter.GetType() == filterType);
    ASSERT_TRUE(searchFilter.GetRule() == filterRule);
}

TEST_F(MainWindowViewModelTests, Can_remove_an_existing_search_filter)
{
    // Arrange
    MainWindowViewModel sut;

    int searchFilterTypeIndex = 0;
    int searchFilterRuleIndex = 0;
    QString filterValue = "Stanley";
    sut.addNewFilter(searchFilterTypeIndex, searchFilterRuleIndex, filterValue);

    // Act
    bool filterRemoved = sut.removeSelectedFilter(0);

    // Assert
    ASSERT_TRUE(filterRemoved);

    auto filtersModel = sut.searchFiltersModel();
    ASSERT_TRUE(filtersModel->rowCount() == 0);
}

TEST_F(MainWindowViewModelTests, Can_connect_notification_signals)
{
    // Arrange
    MainWindowViewModel sut;

    // Act
    bool searchStartedConnected = QObject::connect(&sut, &MainWindowViewModel::searchStarted, []() {});
    bool searchFinishedConnected = QObject::connect(&sut, &MainWindowViewModel::searchFinished, [](bool errorOccurred) {});
    bool contactFoundConnected = QObject::connect(&sut, &MainWindowViewModel::contactFound, []() {});

    // Assert
    bool allSignalsConnected = searchStartedConnected && searchFinishedConnected && contactFoundConnected;
    ASSERT_TRUE(allSignalsConnected);
}

TEST_F(MainWindowViewModelTests, Can_search)
{
    // Arrange
    MainWindowViewModel sut;
    auto contactsModel = sut.contactsModel();
    contactsModel->Clear();
    int rowCountBeforeSearch = contactsModel->rowCount();

    auto connectionSettings = GetConnectionSettingsForTestAdLdsInstance();
    auto settingsKeeper = GetDependencyResolver()->GetSettingsKeeper();
    settingsKeeper->SetConnectionSettings(connectionSettings);

    bool searchStartedSignalReceived = false;
    QObject::connect( &sut, &MainWindowViewModel::searchStarted,
        [&searchStartedSignalReceived]() { searchStartedSignalReceived = true; }
    );

    bool errorOccurredDuringSearch = false;
    bool searchFinishedSignalReceived = false;
    QObject::connect( &sut, &MainWindowViewModel::searchFinished,
        [&searchFinishedSignalReceived, &errorOccurredDuringSearch](bool errorOccurred) {
            errorOccurredDuringSearch = errorOccurred;
            searchFinishedSignalReceived = true;
        }
    );

    bool contactFoundSignalReceived = false;
    QObject::connect( &sut, &MainWindowViewModel::contactFound,
        [&contactFoundSignalReceived]() { contactFoundSignalReceived = true; }
    );

    // Act
    bool searchStarted = sut.startSearching();
    sut.WaitUntilSearchIsFinished();
    int rowCountAfterSearch = contactsModel->rowCount();

    // Assert
    ASSERT_TRUE(searchStarted);
    ASSERT_FALSE(errorOccurredDuringSearch);
    ASSERT_TRUE(searchStartedSignalReceived);
    ASSERT_TRUE(searchFinishedSignalReceived);
    ASSERT_TRUE(contactFoundSignalReceived);
    ASSERT_EQ(rowCountBeforeSearch, 0);
    ASSERT_THAT(rowCountAfterSearch, Gt(0));
}

TEST_F(MainWindowViewModelTests, Provides_error_description_when_error_occurred_during_search)
{
    // Arrange
    MainWindowViewModel sut;

    auto invalidConnectionSettings = GetInvalidConnectionSettings();
    auto settingsKeeper = GetDependencyResolver()->GetSettingsKeeper();
    settingsKeeper->SetConnectionSettings(invalidConnectionSettings);

    bool lastErrorChanged = false;
    QObject::connect(&sut, &MainWindowViewModel::lastErrorChanged,
        [&lastErrorChanged]() { lastErrorChanged = true; }
    );

    // Act
    bool searchStarted = sut.startSearching();
    sut.WaitUntilSearchIsFinished();

    // Assert
    ASSERT_TRUE(lastErrorChanged);
    ASSERT_FALSE(searchStarted);
    QString lastError = sut.lastError();
    ASSERT_FALSE(lastError.isEmpty());
}


TEST_F(MainWindowViewModelTests, Refresh_contacts_details_when_user_selects_a_contact)
{
    MainWindowViewModel sut;

    auto connectionSettings = GetConnectionSettingsForTestAdLdsInstance();
    auto settingsKeeper = GetDependencyResolver()->GetSettingsKeeper();
    settingsKeeper->SetConnectionSettings(connectionSettings);

    // Act
    bool searchStarted = sut.startSearching();
    sut.WaitUntilSearchIsFinished();
    int contactIndex = 0;
    sut.handleContactSelected(contactIndex);

    // Assert
    auto contactFromContactList = sut.contactsModel()->GetContact(0);
    auto contactFromContactDetails = sut.contactDetailsModel()->GetContact();
    ASSERT_EQ(contactFromContactList, contactFromContactDetails);
}

