#pragma once
#include "gtest/gtest.h"
#include "../AdBookBL/export.h"
#include "../QmlAdBookBL/export.h"


class TestSuiteBase : public ::testing::Test
{
public:
    TestSuiteBase();
    virtual ~TestSuiteBase();
protected:
    static std::wstring GetLdapUrlForTestAdLdsInstance();
    static std::wstring GetLdapUrlForTestPersonOnAdLdsInstance();
    static qmladbook::ConnectionSettings GetConnectionSettingsForTestAdLdsInstance();
    static qmladbook::ConnectionSettings GetInvalidConnectionSettings();
    static std::vector<qmladbook::Contact> CreateContactsForTesting(size_t numberOfContacts = 8);
};

