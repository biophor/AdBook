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

#include "gtest/gtest.h"

#include "../AdBookBL/export.h"
#include "../QmlAdBookBL/export.h"

using namespace qmladbook;
using namespace adbook;

class ConnectionSettingsTests : public ::testing::Test {
public:
    void SetUp() override {
    }
    void TearDown() override {
    }
};

TEST_F(ConnectionSettingsTests, Can_be_coverted_to_AdbookConnectionParams)
{
    // Arrange
    using namespace qmladbook;
    ConnectionSettings sut;
    QString domain = "domain", login = "login", passwordString = "abcd123";
    sut.ConnectAsCurrentUser(false);
    sut.ConnectCurrentDomain(false);
    sut.SetAddress(domain);
    sut.SetLogin(login);
    Password password;
    password.TryToSet(passwordString);
    sut.SetPassword(password);

    // Act
    adbook::ConnectionParams params = sut;

    // Assert
    EXPECT_EQ(params.GetAddress(), domain.toStdWString());
    EXPECT_EQ(params.GetLogin(), login.toStdWString());
    EXPECT_EQ(params.GetPassword(), passwordString.toStdWString());
    EXPECT_EQ(params.Get_ConnectAsCurrentUser(), sut.ConnectAsCurrentUser());
    EXPECT_EQ(params.Get_ConnectDomainYouLoggedIn(), sut.ConnectCurrentDomain());
}

