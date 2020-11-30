#include "gtest/gtest.h"

#include "../AdBookBL/export.h"
#include "../QmlAdBookBL/export.h"

using namespace qmladbook;

class SettingsCheckerTests : public ::testing::Test {
public:
    void SetUp() override {
        CoInitialize(nullptr);
    }
    void TearDown() override {
        CoUninitialize();
    }
};

TEST_F(SettingsCheckerTests, Can_succesfuly_validate_correct_settings)
{
    // Arrange
    auto adAccessFactory = adbook::GetAdAccessFactory();
    SettingsChecker sut(adAccessFactory);
    ConnectionSettings connectionSettings;
    connectionSettings.ConnectCurrentDomain(false);
    connectionSettings.ConnectAsCurrentUser(true);
    connectionSettings.SetAddress("LDAP://localhost:55555/CN=users,DC=testdata,DC=adbook,DC=local");

    // Act
    ASettingsChecker::CheckResult checkResult = sut.Check(connectionSettings);

    // Assert
    ASSERT_TRUE(ASettingsChecker::CheckResultStatus::Ok == checkResult.status);
}

TEST_F(SettingsCheckerTests, Can_determine_if_settings_are_invalid)
{
    // Arrange
    auto adAccessFactory = adbook::GetAdAccessFactory();
    SettingsChecker sut(adAccessFactory);
    ConnectionSettings connectionSettings;
    connectionSettings.ConnectCurrentDomain(false);
    connectionSettings.ConnectAsCurrentUser(true);
    connectionSettings.SetAddress("LDAP://invalid_domain_name:55555/CN=invalid");

    // Act
    ASettingsChecker::CheckResult checkResult = sut.Check(connectionSettings);

    // Assert
    ASSERT_TRUE(ASettingsChecker::CheckResultStatus::FailedToConnect == checkResult.status);
    ASSERT_FALSE(checkResult.details.isEmpty()); // details must contains the error's description
}

