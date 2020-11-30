#pragma once
#include <vector>
#include "gtest/gtest.h"

#include "../AdBookBL/export.h"
#include "../QmlAdBookBL/export.h"


class TestSuiteBase : public ::testing::Test
{
public:
    TestSuiteBase();
    virtual ~TestSuiteBase();
protected:
    static qmladbook::Contact CreateContactForTesting();

    static std::vector<qmladbook::Contact> CreateContactsForTesting(size_t numberOfContacts = 8);
};


