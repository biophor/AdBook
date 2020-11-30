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

class ContactDetailsModelTests : public TestSuiteBase
{
public:
    void SetUp() override {
    }
    void TearDown() override {
    }
};

TEST_F(ContactDetailsModelTests, Model_can_be_filled_up_with_contact_attributes)
{
    // Arrange
    ContactDetailsModel sut;
    Contact contact = CreateContactForTesting();

    // Act
    sut.SetContact(contact);

    // Assert
    // perform per-attribute comparison
    auto & attrTraits = adbook::Attributes::GetInstance();
    auto attrIds = adbook::Attributes::GetInstance().GetAttrIds();
    for (auto id : attrIds) {
        if (attrTraits.IsString(id)) {  // model doesn't support binary attributes
            QString attrValueFromModel = sut.GetAttrValue(id);
            QString attrValueFromContact = std::get<QString>(contact.GetAttr(id));
            EXPECT_EQ(attrValueFromModel, attrValueFromContact);
        }
    }
}

TEST_F(ContactDetailsModelTests, Model_provides_data_for_QT)
{
    // Arrange
    ContactDetailsModel sut;
    Contact contact = CreateContactForTesting();
    sut.SetContact(contact);

    // Act && Assert
    for (int rowIndex = 0; rowIndex < sut.rowCount(); ++rowIndex) {
        for (int role : sut.roleNames().keys()) {
            // QT reads the model data via 'data()'
            QVariant v = sut.data(sut.index(rowIndex), role);

            ASSERT_EQ(v.type(), QVariant::String);
            ASSERT_FALSE(v.toString().isEmpty());
        }
    }
}

TEST_F(ContactDetailsModelTests, Model_provides_data_by_row_index)
{
    // Arrange
    ContactDetailsModel sut;
    Contact contact = CreateContactForTesting();
    sut.SetContact(contact);

    // Act & Assert
    for (int rowIndex = 0; rowIndex < sut.rowCount(); ++rowIndex) {
        adbook::Attributes::AttrId attrId = sut.GetAttrId(rowIndex);
        // Assert
        EXPECT_EQ(std::get<QString>(contact.GetAttr(attrId)), sut.GetAttrValue(rowIndex));
    }
}
TEST_F(ContactDetailsModelTests, Clearing_the_model)
{
    // Arrange
    ContactDetailsModel sut;
    Contact contact = CreateContactForTesting();
    sut.SetContact(contact);

    // Act
    sut.Clear();

    // Assert
    for (int rowIndex = 0; rowIndex < sut.rowCount(); ++rowIndex) {
        EXPECT_TRUE(sut.GetAttrValue(rowIndex).isEmpty());
    }
}

TEST_F(ContactDetailsModelTests, Model_provides_header_information_for_QT)
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

