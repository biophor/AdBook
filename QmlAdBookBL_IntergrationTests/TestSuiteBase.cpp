#include <objbase.h>
#include "TestSuiteBase.h"

using namespace adbook;
using namespace qmladbook;

TestSuiteBase::TestSuiteBase() {
    CoInitialize(nullptr);
}

TestSuiteBase::~TestSuiteBase() {
    CoUninitialize();
}

std::wstring TestSuiteBase::GetLdapUrlForTestAdLdsInstance() {
    return L"LDAP://localhost:55555/CN=users,DC=testdata,DC=adbook,DC=local";
}

std::wstring TestSuiteBase::GetLdapUrlForTestPersonOnAdLdsInstance() {
    return L"LDAP://localhost:55555/CN=Kristie Etherton,CN=users,DC=testdata,DC=adbook,DC=local";
}

qmladbook::ConnectionSettings TestSuiteBase::GetConnectionSettingsForTestAdLdsInstance()
{
    qmladbook::ConnectionSettings settings;
    settings.ConnectCurrentDomain(false);
    settings.ConnectAsCurrentUser(true);
    settings.SetAddress("LDAP://localhost:55555/CN=users,DC=testdata,DC=adbook,DC=local");
    return settings;
}

qmladbook::ConnectionSettings TestSuiteBase::GetInvalidConnectionSettings() {
    qmladbook::ConnectionSettings settings;
    settings.ConnectCurrentDomain(false);
    settings.ConnectAsCurrentUser(true);
    settings.SetAddress("LDAP://localhost:55555/CN=INVALID,DC=testdata,DC=adbook,DC=local");
    return settings;
}

std::vector<qmladbook::Contact> TestSuiteBase::CreateContactsForTesting(size_t numberOfContacts)
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