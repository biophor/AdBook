#include <vector>
#include "gtest/gtest.h"

#include "../AdBookBL/export.h"
#include "../QmlAdBookBL/export.h"
#include "TestSuiteBase.h"

using namespace qmladbook;

class ContactsKeeperTests : public TestSuiteBase {
public:
    void SetUp() override {
    }
    void TearDown() override {
    }

protected:
    static void PopulateContactsModelForTesting(ContactsModel & model);
};

void ContactsKeeperTests::PopulateContactsModelForTesting(ContactsModel & model)
{
    model.Clear();
    for (auto && c : CreateContactsForTesting(10)) {
        model.AddContact(std::move(c));
    }
}

TEST_F(ContactsKeeperTests, Can_save_and_load_contacts)
{
    // Arrange
    ContactsModel contactsModel1, contactsModel2, emptyModel;
    ContactsKeeper sut;
    PopulateContactsModelForTesting(contactsModel1);

    // Act
    sut.Save(&emptyModel);
    sut.Save(&contactsModel1);
    sut.Load(&contactsModel2);

    // Assert
    ASSERT_EQ(contactsModel1, contactsModel2);
}

