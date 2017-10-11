#include "pch.h"

// see the document 'How to setup the test environment on Windows 10.txt'

class AdSearchTests: public ::testing::Test{
public:
    void SetUp() override {
        CoInitialize(NULL);
    }
    void TearDown() override {
        CoUninitialize();
    }
};

TEST_F(AdSearchTests, CanFindExistingPerson) {
    // Arrange
    std::shared_ptr<adbook::AbstractAdAccessFactory> factory =
        adbook::CreateAdAccessFactory(adbook::AbstractAdAccessFactory::ProductsType::Real);

    std::unique_ptr<adbook::AbstractAdSearcher> searcher =
        factory->CreateSearcher();

    adbook::LdapRequestBuilder requestBuilder;
    requestBuilder.AddRule(L"cn", adbook::LdapRequestBuilder::ExactMatch, L"Ramon Linden");

    adbook::ConnectionParams connectionParams;
    connectionParams.ConnectDomainYouAreLoggedIn(false);
    connectionParams.UseCurrentUserCredentials(true);
    connectionParams.SetLdapPath(L"LDAP://localhost:55555/CN=users,DC=testdata,DC=adbook,DC=local");

    // Act
    bool startedCallbackCalled = false;
    bool finishedCallbackCalled = false;
    bool itemFoundCallbackCalled = false;
    adbook::AdPersonDesc person;
    searcher->SetCallbacks(
        [&itemFoundCallbackCalled, &person](adbook::AdPersonDesc && rp) { itemFoundCallbackCalled = true; person = std::move(rp); },
        [&startedCallbackCalled]() {startedCallbackCalled = true; },
        [&finishedCallbackCalled]() {finishedCallbackCalled = true; }
    );
    searcher->Start(requestBuilder.Get(), connectionParams);
    searcher->Wait();
    // Assert
    ASSERT_TRUE(person.GetStringAttr(L"cn") == L"Ramon Linden");
    ASSERT_TRUE(startedCallbackCalled);
    ASSERT_TRUE(finishedCallbackCalled);
    ASSERT_TRUE(itemFoundCallbackCalled);
}


TEST_F(AdSearchTests, CantFindNonExistingPerson) {
    // Arrange
    std::shared_ptr<adbook::AbstractAdAccessFactory> factory =
        adbook::CreateAdAccessFactory(adbook::AbstractAdAccessFactory::ProductsType::Real);

    std::unique_ptr<adbook::AbstractAdSearcher> searcher =
        factory->CreateSearcher();

    adbook::LdapRequestBuilder requestBuilder;
    requestBuilder.AddRule(L"cn", adbook::LdapRequestBuilder::ExactMatch, L"Invalid Name");

    adbook::ConnectionParams connectionParams;
    connectionParams.ConnectDomainYouAreLoggedIn(false);
    connectionParams.UseCurrentUserCredentials(true);
    connectionParams.SetLdapPath(L"LDAP://localhost:55555/CN=users,DC=testdata,DC=adbook,DC=local");

    // Act
    bool startedCallbackCalled = false;
    bool finishedCallbackCalled = false;
    bool itemFoundCallbackCalled = false;
    adbook::AdPersonDesc person;
    searcher->SetCallbacks(
        [&itemFoundCallbackCalled, &person](adbook::AdPersonDesc && rp) { itemFoundCallbackCalled = true; person = std::move(rp); },
        [&startedCallbackCalled]() {startedCallbackCalled = true; },
        [&finishedCallbackCalled]() {finishedCallbackCalled = true; }
    );
    searcher->Start(requestBuilder.Get(), connectionParams);
    searcher->Wait();
    // Assert
    ASSERT_TRUE(startedCallbackCalled);
    ASSERT_TRUE(finishedCallbackCalled);
    ASSERT_FALSE(itemFoundCallbackCalled);
}
