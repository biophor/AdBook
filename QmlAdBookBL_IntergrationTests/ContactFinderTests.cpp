#include "gtest/gtest.h"

#include "../AdBookBL/export.h"
#include "../QmlAdBookBL/export.h"
#include "TestSuiteBase.h"

using namespace qmladbook;

class ContactFinderTests : public TestSuiteBase {
public:
    void SetUp() override {
    }
    void TearDown() override {
    }
};

TEST_F(ContactFinderTests, Can_find_an_existing_Contact)
{
    // Arrange
    adbook::LdapRequestBuilder requestBuilder;
    requestBuilder.AddObjectCategoryRule();
    requestBuilder.AddRule(L"cn", adbook::LdapRequestBuilder::ExactMatch, L"Ramon Linden");
    requestBuilder.AddAND();
    ConnectionSettings connectionSettings = GetConnectionSettingsForTestAdLdsInstance();

    auto resolver = qmladbook::GetDependencyResolver();
    ContactFinder contactFinder(resolver->GetAdFactory());

    bool startedCallbackCalled = false;
    bool finishedCallbackCalled = false;
    bool errorOccurredCallbackCalled = false;
    size_t numContactsFound = 0;

    auto contactFoundCallback = [&numContactsFound]() { ++numContactsFound; };
    QObject::connect(&contactFinder, &ContactFinder::contactFound,
        contactFoundCallback
    );
    auto searchStartedCallback = [&startedCallbackCalled]() { startedCallbackCalled = true; };
    QObject::connect(&contactFinder, &ContactFinder::searchStarted,
        searchStartedCallback
    );
    auto searchFinishedCallback = [&finishedCallbackCalled]() { finishedCallbackCalled = true; };
    QObject::connect(&contactFinder, &ContactFinder::searchFinished,
        searchFinishedCallback
    );

    auto errorOccurredCallback = [&errorOccurredCallbackCalled](QString ) { errorOccurredCallbackCalled = true; };
    QObject::connect(&contactFinder, &ContactFinder::errorOccurred,
        errorOccurredCallback
    );

    QString ldapRequest = QString::fromStdWString(requestBuilder.Get());

    // Act
    contactFinder.Start(ldapRequest, connectionSettings);
    contactFinder.Wait();

    // Assert
    ASSERT_NO_THROW(contactFinder.PropogateLastSearchException());
    ASSERT_TRUE(startedCallbackCalled);
    ASSERT_TRUE(finishedCallbackCalled);
    ASSERT_FALSE(errorOccurredCallbackCalled);
    ASSERT_EQ(numContactsFound, 1);
}

TEST_F(ContactFinderTests, Cannot_find_a_non_existent_Contact)
{
    // Arrange
    adbook::LdapRequestBuilder requestBuilder;
    requestBuilder.AddObjectCategoryRule();
    requestBuilder.AddRule(L"cn", adbook::LdapRequestBuilder::ExactMatch, L"Invalid Person");
    requestBuilder.AddAND();
    ConnectionSettings connectionSettings = GetConnectionSettingsForTestAdLdsInstance();
    ContactFinder contactFinder(qmladbook::GetDependencyResolver()->GetAdFactory());

    bool startedCallbackCalled = false;
    bool finishedCallbackCalled = false;
    bool errorOccurredCallbackCalled = false;
    size_t numContactsFound = 0;

    auto contactFoundCallback = [&numContactsFound]() { ++numContactsFound; };
    QObject::connect(&contactFinder, &ContactFinder::contactFound,
        contactFoundCallback
    );
    auto searchStartedCallback = [&startedCallbackCalled]() { startedCallbackCalled = true; };
    QObject::connect(&contactFinder, &ContactFinder::searchStarted,
        searchStartedCallback
    );
    auto searchFinishedCallback = [&finishedCallbackCalled]() { finishedCallbackCalled = true; };
    QObject::connect(&contactFinder, &ContactFinder::searchFinished,
        searchFinishedCallback
    );
    auto errorOccurredCallback = [&errorOccurredCallbackCalled](QString) { errorOccurredCallbackCalled = true; };
    QObject::connect(&contactFinder, &ContactFinder::errorOccurred,
        searchFinishedCallback
    );
    QString ldapRequest = QString::fromStdWString(requestBuilder.Get());

    // Act
    contactFinder.Start(ldapRequest, connectionSettings);
    contactFinder.Wait();

    // Assert
    ASSERT_NO_THROW(contactFinder.PropogateLastSearchException());
    ASSERT_TRUE(startedCallbackCalled);
    ASSERT_TRUE(finishedCallbackCalled);
    ASSERT_FALSE(errorOccurredCallbackCalled);
    ASSERT_EQ(numContactsFound, 0);
}

TEST_F(ContactFinderTests, Throws_an_exception_when_an_invalid_search_filter_passed_in)
{
    // Arrange
    ConnectionSettings connectionSettings = GetConnectionSettingsForTestAdLdsInstance();

    auto resolver = qmladbook::GetDependencyResolver();
    ContactFinder contactFinder(resolver->GetAdFactory());

    bool startedCallbackCalled = false;
    bool finishedCallbackCalled = false;
    bool errorOccurredCallbackCalled = false;
    size_t numContactsFound = 0;

    auto contactFoundCallback = [&numContactsFound]() { ++numContactsFound; };
    QObject::connect(&contactFinder, &ContactFinder::contactFound,
        contactFoundCallback
    );
    auto searchStartedCallback = [&startedCallbackCalled]() { startedCallbackCalled = true; };
    QObject::connect(&contactFinder, &ContactFinder::searchStarted,
        searchStartedCallback
    );
    auto searchFinishedCallback = [&finishedCallbackCalled]() { finishedCallbackCalled = true; };
    QObject::connect(&contactFinder, &ContactFinder::searchFinished,
        searchFinishedCallback
    );

    auto errorOccurredCallback = [&errorOccurredCallbackCalled](QString) { errorOccurredCallbackCalled = true; };
    QObject::connect(&contactFinder, &ContactFinder::errorOccurred,
        errorOccurredCallback
    );

    QString ldapRequest = "INVALID SEARCH FILTER";

    // Act
    contactFinder.Start(ldapRequest, connectionSettings);
    contactFinder.Wait();

    // Assert
    ASSERT_THROW(contactFinder.PropogateLastSearchException(), adbook::HrError);
    ASSERT_TRUE(startedCallbackCalled);
    ASSERT_TRUE(finishedCallbackCalled);
    ASSERT_TRUE(errorOccurredCallbackCalled);
    ASSERT_EQ(numContactsFound, 0);
}

