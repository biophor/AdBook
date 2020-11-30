#pragma once

#include "gtest/gtest.h"

#include "../AdBookBL/export.h"
#include "../QmlAdBookBL/export.h"


class QmlAdBookTestSuiteBase : public ::testing::Test
{
public:
protected:
    static qmladbook::ConnectionSettings GetConnectionSettingsForTestAdLdsInstance();
};

