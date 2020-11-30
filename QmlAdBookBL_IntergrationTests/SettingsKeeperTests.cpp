#include "gtest/gtest.h"

#include "../AdBookBL/export.h"
#include "../QmlAdBookBL/export.h"
#include "TestSuiteBase.h"
using namespace qmladbook;

class SettingsKeeperTests : public TestSuiteBase {
public:
    void SetUp() override {
    }
    void TearDown() override {
    }
};

TEST_F(SettingsKeeperTests, Can_save_and_load_settings)
{
    // Arrange
    ConnectionSettings connectionSettings1, connectionSettings2, emptyConnectionSettings;
    connectionSettings1.ConnectAsCurrentUser(false);
    connectionSettings1.ConnectCurrentDomain(false);
    connectionSettings1.SetAddress("address");
    connectionSettings1.SetLogin("login");
    Password password;
    password.TryToSet("plain_text_password");
    connectionSettings1.SetPassword(password);

    SettingsKeeper sut;

    QString appName = "acme_app", orgName = "acme_org";

    // Act
    sut.SetConnectionSettings(emptyConnectionSettings);
    sut.Save(orgName, appName);

    sut.SetConnectionSettings(connectionSettings1);
    sut.Save(orgName, appName);

    sut.Load(orgName, appName);
    connectionSettings2 = sut.GetConnectionSettings();

    // Assert
    QString p1, p2;
    bool b1 = connectionSettings1.GetPassword().TryToGetAsPlainTextString(p1);
    bool b2 = connectionSettings2.GetPassword().TryToGetAsPlainTextString(p2);
    ASSERT_TRUE(connectionSettings1 == connectionSettings2);
}
