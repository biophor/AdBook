#include "pch.h"
#include "TestSuiteBase.h"

std::wstring TestSuiteBase::GetLdapUrlForTestAdLdsInstance() {
    return L"LDAP://localhost:55555/CN=users,DC=testdata,DC=adbook,DC=local";
}

std::wstring TestSuiteBase::GetLdapUrlForTestPersonOnAdLdsInstance() {
    return L"LDAP://localhost:55555/CN=Kristie Etherton,CN=users,DC=testdata,DC=adbook,DC=local";
}