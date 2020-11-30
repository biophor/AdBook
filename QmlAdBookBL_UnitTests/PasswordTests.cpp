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

class PasswordTests : public ::testing::Test {
public:
    void SetUp() override {
        CoInitialize(nullptr);
    }
    void TearDown() override {
        CoUninitialize();
    }
};

TEST_F(PasswordTests, Can_handle_plain_text_password)
{
    // Arrange
    QString plainTextPassword1 = "Passw0rd";
    QString plainTextPassword2;
    Password sut1;
    Password sut2;

    // Act
    bool passwordSet = sut1.TryToSet(plainTextPassword1);
    QString serializedPassword = sut1.Serialize();

    sut2.Deserialize(serializedPassword);
    bool passwordGot = sut2.TryToGetAsPlainTextString(plainTextPassword2);

    // Assert
    ASSERT_TRUE(passwordSet);
    ASSERT_TRUE(passwordGot);
    ASSERT_TRUE(plainTextPassword1 == plainTextPassword2);
}


