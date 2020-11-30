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

TEST_F(ContactFinderTests, Can_connect_notification_signals)
{
    // Arrange
    auto resolver = qmladbook::GetDependencyResolver();
    ContactFinder contactFinder(resolver->GetAdFactory());

    // Act
    auto contactFoundCallback = []() { };
    bool contactFoundConnected = QObject::connect(&contactFinder, &ContactFinder::contactFound,
        contactFoundCallback
    );
    auto searchStartedCallback = []() { };
    bool searchStartedConnected = QObject::connect(&contactFinder, &ContactFinder::searchStarted,
        searchStartedCallback
    );
    auto searchFinishedCallback = []() { };
    bool searchFinishedConnected = QObject::connect(&contactFinder, &ContactFinder::searchFinished,
        searchFinishedCallback
    );
    auto errorOccurredCallback = [](QString) { };
    bool errorOccurredConnected = QObject::connect(&contactFinder, &ContactFinder::errorOccurred,
        errorOccurredCallback
    );

    // Assert
    bool allSignalsConnected = contactFoundConnected && searchStartedConnected && searchFinishedConnected  &&
        errorOccurredConnected;

    ASSERT_TRUE(allSignalsConnected);
}

