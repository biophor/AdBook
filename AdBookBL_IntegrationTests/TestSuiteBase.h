#pragma once


class TestSuiteBase : public ::testing::Test
{
public:
protected:
    static std::wstring GetLdapUrlForTestAdLdsInstance();
    static std::wstring GetLdapUrlForTestPersonOnAdLdsInstance();
};

