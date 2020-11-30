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

class ContactTests : public ::testing::Test {
public:
    void SetUp() override {

    }
    void TearDown() override {

    }
    static Contact CreateContactForTesting();
    static AdPersonDesc CreateAdPersonForTesting();
};

adbook::AdPersonDesc ContactTests::CreateAdPersonForTesting()
{
    adbook::AdPersonDesc adp;
    auto & attrTraits = adbook::Attributes::GetInstance();

    for (auto id : attrTraits.GetAttrIds()) {
        if (attrTraits.IsString(id)) {
            QString stringAttrValue = QString("attr value %1").arg(id);
            adp.SetStringAttr(attrTraits.GetLdapAttrNamePtr(id), stringAttrValue.toStdWString());
        }
        else {
            adbook::BinaryAttrVal binaryAttrValue{ id };
            adp.SetBinaryAttr(attrTraits.GetLdapAttrNamePtr(id), binaryAttrValue);
        }
    }
    return adp;
}

Contact ContactTests::CreateContactForTesting()
{
    Contact contact;
    auto & attrTraits = adbook::Attributes::GetInstance();
    auto attrIds = attrTraits.GetAttrIds();
    for (auto id : attrIds) {
        if (attrTraits.IsString(id)) {
            QString stringAttrValue = QString("attr value %1").arg(id);
            contact.SetAttr(id, stringAttrValue);
        }
        else {
            adbook::BinaryAttrVal binaryAttrValue{ id };
            contact.SetAttr(id, binaryAttrValue);
        }
    }
    return contact;
}

TEST_F(ContactTests, Contact_can_be_constructed_from_an_existing_AdPerson)
{
    // Arrange
    adbook::AdPersonDesc adp = CreateAdPersonForTesting();

    // Act
    Contact contact(std::move(adp));

    // Assert
    // perform per-attribute comparison 'contact' and 'adp'
    auto & attrTraits = adbook::Attributes::GetInstance();
    for (auto id : attrTraits.GetAttrIds()) {
        if (attrTraits.IsString(id)) {
            QString contactAttrValue = std::get<QString>(contact.GetAttr(id));
            const wchar_t * adpAttrValue = adp.GetStringAttrPtr(attrTraits.GetLdapAttrNamePtr(id));
            EXPECT_EQ(contactAttrValue, adpAttrValue);
        }
        else {
            adbook::BinaryAttrVal contactAttrValue = std::get<adbook::BinaryAttrVal>(contact.GetAttr(id));
            adbook::BinaryAttrVal adpAttrValue = adp.GetBinaryAttr(attrTraits.GetLdapAttrNamePtr(id));
            EXPECT_EQ(contactAttrValue, adpAttrValue);
        }
    }
}

TEST_F(ContactTests, Contact_can_be_converted_to_AdPerson)
{
    // Arrange
    Contact contact = CreateContactForTesting();

    // Act
    adbook::AdPersonDesc adp = contact;

    // Assert
    // perform per-attribute comparison 'contact' and 'adp'
    auto & attrTraits = adbook::Attributes::GetInstance();
    for (auto id : attrTraits.GetAttrIds()) {
        if (attrTraits.IsString(id)) {
            QString contactAttrValue = std::get<QString>(contact.GetAttr(id));
            const wchar_t * adpAttrValue = adp.GetStringAttrPtr(attrTraits.GetLdapAttrNamePtr(id));
            EXPECT_EQ(contactAttrValue, adpAttrValue);
        }
        else {
            adbook::BinaryAttrVal contactAttrValue = std::get<adbook::BinaryAttrVal>(contact.GetAttr(id));
            adbook::BinaryAttrVal adpAttrValue = adp.GetBinaryAttr(attrTraits.GetLdapAttrNamePtr(id));
            EXPECT_EQ(contactAttrValue, adpAttrValue);
        }
    }
}

TEST_F(ContactTests, Contact_can_read_and_write_all_AdBookAttributes)
{
    // Arrange
    Contact contact;

    // Act & Assert
    auto & attrTraits = adbook::Attributes::GetInstance();
    auto attrIds = attrTraits.GetAttrIds();
    for (auto id : attrIds) {
        if (attrTraits.IsString(id)) {
            // read & write string attribute
            QString stringAttrValue = QString("attr value %1").arg(id);
            contact.SetAttr(id, stringAttrValue);
            QString attrValue2 = std::get<QString>(contact.GetAttr(id));
            EXPECT_EQ(stringAttrValue, attrValue2);
        }
        else {
            // read & write binary attribute
            adbook::BinaryAttrVal binaryAttrValue{ id };
            contact.SetAttr(id, binaryAttrValue);
            adbook::BinaryAttrVal binaryAttrValue2 = std::get<adbook::BinaryAttrVal>(contact.GetAttr(id));
            EXPECT_EQ(binaryAttrValue, binaryAttrValue2);
        }
    }
}

