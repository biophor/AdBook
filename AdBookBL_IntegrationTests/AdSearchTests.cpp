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

#include "pch.h"
#include "TestSuiteBase.h"

// see the document 'How to setup the test environment on Windows 10.txt'

class AdSearchTests: public TestSuiteBase {
public:
    void SetUp() override {
        CoInitialize(NULL);
    }
    void TearDown() override {
        CoUninitialize();
    }
};

TEST_F(AdSearchTests, Can_find_an_existing_person) {
    // Arrange
    adbook::AdSearcher searcher;
    adbook::LdapRequestBuilder requestBuilder;
    requestBuilder.AddObjectCategoryRule();
    requestBuilder.AddRule(L"cn", adbook::LdapRequestBuilder::ExactMatch, L"Ramon Linden");
    requestBuilder.AddAND();
    adbook::ConnectionParams connectionParams;
    connectionParams.Set_ConnectDomainYouLoggedIn(false);
    connectionParams.Set_ConnectAsCurrentUser(true);
    connectionParams.SetAddress(GetLdapUrlForTestAdLdsInstance());

    // Act
    bool startedCallbackCalled = false;
    bool finishedCallbackCalled = false;
    bool itemFoundCallbackCalled = false;
    adbook::AdPersonDesc person;
    searcher.SetCallbacks(
        [&itemFoundCallbackCalled, &person](adbook::AdPersonDesc && rp)
            { itemFoundCallbackCalled = true; person = std::move(rp); },
        [&startedCallbackCalled]() {startedCallbackCalled = true; },
        [&finishedCallbackCalled]() {finishedCallbackCalled = true; }
    );
    searcher.Start(requestBuilder.Get(), connectionParams);
    searcher.Wait();
    searcher.PropogateSearchException();

    // Assert
    ASSERT_TRUE(person.GetStringAttr(L"cn") == L"Ramon Linden");
    ASSERT_TRUE(startedCallbackCalled);
    ASSERT_TRUE(finishedCallbackCalled);
    ASSERT_TRUE(itemFoundCallbackCalled);
}

TEST_F(AdSearchTests, Can_find_all_people) {
    // Arrange
    adbook::AdSearcher searcher;

    adbook::LdapRequestBuilder requestBuilder;
    requestBuilder.AddObjectCategoryRule();

    adbook::ConnectionParams connectionParams;
    connectionParams.Set_ConnectDomainYouLoggedIn(false);
    connectionParams.Set_ConnectAsCurrentUser(true);
    connectionParams.SetAddress(GetLdapUrlForTestAdLdsInstance());

    // Act
    bool startedCallbackCalled = false;
    bool finishedCallbackCalled = false;
    bool itemFoundCallbackCalled = false;
    std::vector<adbook::AdPersonDesc> people;

    searcher.SetCallbacks(
        [&itemFoundCallbackCalled, &people](adbook::AdPersonDesc && rp)
            { itemFoundCallbackCalled = true; people.push_back(std::move(rp)); },
        [&startedCallbackCalled]() {startedCallbackCalled = true; },
        [&finishedCallbackCalled]() {finishedCallbackCalled = true; }
    );
    searcher.Start(requestBuilder.Get(), connectionParams);
    searcher.Wait();
    searcher.PropogateSearchException();

    // Assert
    ASSERT_TRUE(people.size() == 5);    // test adlds instance contains 5 person
    ASSERT_TRUE(startedCallbackCalled);
    ASSERT_TRUE(finishedCallbackCalled);
    ASSERT_TRUE(itemFoundCallbackCalled);
}

TEST_F(AdSearchTests, Cannot_find_non_existing_person)
{
    // Arrange
    adbook::AdSearcher searcher;

    adbook::LdapRequestBuilder requestBuilder;
    requestBuilder.AddObjectCategoryRule();
    requestBuilder.AddRule(L"cn", adbook::LdapRequestBuilder::ExactMatch, L"Invalid Name");
    requestBuilder.AddAND();

    adbook::ConnectionParams connectionParams;
    connectionParams.Set_ConnectDomainYouLoggedIn(false);
    connectionParams.Set_ConnectAsCurrentUser(true);
    connectionParams.SetAddress(GetLdapUrlForTestAdLdsInstance());

    // Act
    bool startedCallbackCalled = false;
    bool finishedCallbackCalled = false;
    bool itemFoundCallbackCalled = false;
    adbook::AdPersonDesc person;
    searcher.SetCallbacks(
        [&itemFoundCallbackCalled, &person](adbook::AdPersonDesc && rp) { itemFoundCallbackCalled = true; person = std::move(rp); },
        [&startedCallbackCalled]() {startedCallbackCalled = true; },
        [&finishedCallbackCalled]() {finishedCallbackCalled = true; }
    );
    searcher.Start(requestBuilder.Get(), connectionParams);
    searcher.Wait();
    searcher.PropogateSearchException();

    // Assert
    ASSERT_TRUE(startedCallbackCalled);
    ASSERT_TRUE(finishedCallbackCalled);
    ASSERT_FALSE(itemFoundCallbackCalled);
}

TEST_F(AdSearchTests, Throws_exception_when_ldap_filter_is_invalid)
{
    // Arrange
    adbook::AdSearcher searcher;

    std::wstring invalidLdapRequest = L"invalid ldap request";

    adbook::ConnectionParams connectionParams;
    connectionParams.Set_ConnectDomainYouLoggedIn(false);
    connectionParams.Set_ConnectAsCurrentUser(true);
    connectionParams.SetAddress(GetLdapUrlForTestAdLdsInstance());

    // Act
    searcher.Start(invalidLdapRequest, connectionParams);
    searcher.Wait();

    // Assert
    ASSERT_THROW(searcher.PropogateSearchException(), adbook::HrError);
}

