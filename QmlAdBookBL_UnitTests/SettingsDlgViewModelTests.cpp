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
#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "../AdBookBL/export.h"
#include "../QmlAdBookBL/export.h"

using namespace qmladbook;

class SettingsDlgViewModelTests : public ::testing::Test {
public:
    void SetUp() override {

    }
    void TearDown() override {

    }
};

TEST_F(SettingsDlgViewModelTests, Must_emit_notification_signals_when_properties_are_changed)
{
    // Arrange
    SettingsDlgViewModel sut;

    bool addressChangedCalled = false;
    bool addressChangedSignalConnected = QObject::connect(
        &sut, &SettingsDlgViewModel::addressChanged,
        [&addressChangedCalled]() { addressChangedCalled = true; }
    );
    bool loginChangedCalled = false;
    bool loginChangedSignalConnected = QObject::connect(
        &sut, &SettingsDlgViewModel::loginChanged,
        [&loginChangedCalled]() { loginChangedCalled = true; }
    );

    bool passwordChangedCalled = false;
    bool passwordChangedSignalConnected = QObject::connect(
        &sut, &SettingsDlgViewModel::passwordChanged,
        [&passwordChangedCalled]() { passwordChangedCalled = true; }
    );

    bool connectCurrentDomainChanged = false;
    bool connectCurrentDomainChangedSignalConnected = QObject::connect(
        &sut, &SettingsDlgViewModel::connectCurrentDomainChanged,
        [&connectCurrentDomainChanged]() { connectCurrentDomainChanged = true; }
    );

    bool connectAsCurrentUserChanged = false;
    bool connectAsCurrentUserChangedSignalConnected = QObject::connect(
        &sut, &SettingsDlgViewModel::connectAsCurrentUserChanged,
        [&connectAsCurrentUserChanged]() { connectAsCurrentUserChanged = true; }
    );

    // Act
    sut.setConnectAsCurrentUser(false);
    sut.setConnectCurrentDomain(false);
    sut.setLogin("login");
    sut.setPassword("silly_password");
    sut.setAddress("address");

    // Assert
    ASSERT_TRUE(addressChangedSignalConnected);
    ASSERT_TRUE(loginChangedSignalConnected);
    ASSERT_TRUE(passwordChangedSignalConnected);
    ASSERT_TRUE(connectCurrentDomainChangedSignalConnected);
    ASSERT_TRUE(connectAsCurrentUserChangedSignalConnected);

    ASSERT_TRUE(addressChangedCalled);
    ASSERT_TRUE(loginChangedCalled);
    ASSERT_TRUE(passwordChangedCalled);
    ASSERT_TRUE(connectCurrentDomainChanged);
    ASSERT_TRUE(connectAsCurrentUserChanged);
}

