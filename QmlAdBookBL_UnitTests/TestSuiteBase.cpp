#include "TestSuiteBase.h"

using namespace qmladbook;
using namespace adbook;

TestSuiteBase::TestSuiteBase() = default;
TestSuiteBase::~TestSuiteBase() = default;

qmladbook::Contact TestSuiteBase::CreateContactForTesting()
{
    Contact contact;
    auto & attrTraits = adbook::Attributes::GetInstance();
    auto attrIds = adbook::Attributes::GetInstance().GetAttrIds();
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

std::vector<Contact> TestSuiteBase::CreateContactsForTesting(size_t numberOfContacts)
{
    std::vector<Contact> contacts;
    for (size_t i = 0; i < numberOfContacts; ++i) {
        Contact contact;
        auto & attrTraits = adbook::Attributes::GetInstance();
        auto attrIds = adbook::Attributes::GetInstance().GetAttrIds();
        for (auto id : attrIds) {
            if (id == adbook::Attributes::CommonName) {
                QString commonName = QString("person number %1").arg(i);
                contact.SetAttr(adbook::Attributes::CommonName, commonName);
                continue;
            }

            if (attrTraits.IsString(id)) {
                QString stringAttrValue = QString("attr value %1").arg(id);
                contact.SetAttr(id, stringAttrValue);
            }
            else {
                adbook::BinaryAttrVal binaryAttrValue{ id };
                contact.SetAttr(id, binaryAttrValue);
            }
        }
        contacts.push_back(contact);
    }
    return contacts;
}

