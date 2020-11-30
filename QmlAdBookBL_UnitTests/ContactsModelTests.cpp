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
#include "TestSuiteBase.h"

using namespace qmladbook;
using namespace adbook;

class ContactsModelTests : public TestSuiteBase
{
public:
    void SetUp() override {
    }
    void TearDown() override {
    }
};

TEST_F(ContactsModelTests, Can_add_a_new_contact)
{
    // Arrange
    ContactsModel sut;

    // Act
    Contact contact = CreateContactForTesting();
    sut.AddContact(contact);

    // Assert
    int numContacts = sut.rowCount();
    ASSERT_EQ(numContacts, 1);
}

TEST_F(ContactsModelTests, Can_clear_all_contacts)
{
    // Arrange
    ContactsModel sut;
    sut.AddContact(CreateContactForTesting());
    sut.AddContact(CreateContactForTesting());

    // Act
    sut.Clear();

    // Assert
    int numContacts = sut.rowCount();
    ASSERT_EQ(numContacts, 0);
}

TEST_F(ContactsModelTests, Can_change_an_existing_contact)
{
    // Arrange
    ContactsModel sut;
    Contact contact = CreateContactForTesting();
    contact.SetAttr(adbook::Attributes::Title, "Punisher");
    sut.AddContact(contact);

    // Act
    contact.SetAttr(adbook::Attributes::Title, "Executor");
    sut.SetContact(0, contact);

    // Assert
    ASSERT_EQ(sut.GetContact(0).GetAttrAsString(adbook::Attributes::Title), "Executor");
}

TEST_F(ContactsModelTests, Model_provides_header_information_for_QT)
{
    // Arrange
    ContactsModel sut;

    // Act & Assert
    for (int role : sut.roleNames().keys())
    {
        QVariant v = sut.headerData(0, Qt::Horizontal, role);
        ASSERT_EQ(v.type(), QVariant::String);

        QString s = v.toString();
        ASSERT_FALSE(s.isEmpty());
    }
}

TEST_F(ContactsModelTests, Model_provides_data_for_QT)
{
    // Arrange
    ContactsModel sut;
    auto roles = sut.roleNames();
    for (auto && c : CreateContactsForTesting(10)) {
        sut.AddContact(std::move(c));
    }

    // Act
    for (int rowIndex = 0; rowIndex < sut.rowCount(); ++rowIndex) {
        for (int role : roles.keys()) {
            QVariant v = sut.data(sut.index(rowIndex), role);
            // Assert
            ASSERT_EQ(v.type(), QVariant::String);
            QString s = v.toString();
            ASSERT_FALSE(s.isEmpty());
        }
    }
}

TEST_F(ContactsModelTests, Can_sort_contacts)
{
    // Arrange
    ContactsModel sut;
    auto roles = sut.roleNames();
    for (auto && c : CreateContactsForTesting()) {
        sut.AddContact(std::move(c));
    }

    // Act
    // iterate through each role (model has a separate role for every contact's attribute)
    for (auto iter = roles.cbegin(); iter != roles.cend(); ++iter)
    {
        QString roleName = iter.value();
        int roleId = iter.key();
        // sort contact using specific role(attribute)
        sut.sort(roleName, Qt::AscendingOrder);

        std::vector<QString> values;
        for (int i = 0; i < sut.rowCount(); ++i) {
            QString contactAttribute = sut.data(sut.index(i), roleId).toString();
            values.push_back(contactAttribute);
        }
        // Assert
        ASSERT_TRUE(std::is_sorted(values.cbegin(), values.cend()));
    }
}
