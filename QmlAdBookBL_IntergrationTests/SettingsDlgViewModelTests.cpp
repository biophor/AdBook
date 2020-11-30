#include <QCoreApplication>
#include "gtest/gtest.h"

#include "../AdBookBL/export.h"
#include "../QmlAdBookBL/export.h"
#include "TestSuiteBase.h"

using namespace qmladbook;

class SettingsDlgViewModelTests : public TestSuiteBase {
public:
    void SetUp() override {
        QCoreApplication::setApplicationName("SettingsDlgViewModelTests_app");
        QCoreApplication::setOrganizationName("SettingsDlgViewModelTests_org");
    }
    void TearDown() override {
    }
};

TEST_F(SettingsDlgViewModelTests, Can_save_and_load_settings)
{
    // Arrange
    SettingsDlgViewModel sut;
    QString login = "login";
    QString password = "password";
    QString address = "LDAP://host:12345/CN=users,DC=testdata,DC=adbook,DC=local";
    sut.setConnectAsCurrentUser(false);
    sut.setConnectCurrentDomain(false);
    sut.setLogin(login);
    sut.setPassword(password);
    sut.setAddress(address);

    // Act
    //  save
    sut.save();
    //  clear
    sut.setConnectAsCurrentUser(true);
    sut.setConnectCurrentDomain(true);
    sut.setAddress("");
    sut.setLogin("");
    sut.setPassword("");
    //  load
    sut.load();

    // Assert
    ASSERT_TRUE(sut.address() == address);
    ASSERT_TRUE(sut.login() == login);
    ASSERT_TRUE(sut.password() == password);
    ASSERT_FALSE(sut.connectAsCurrentUser());
    ASSERT_FALSE(sut.connectCurrentDomain());
}

TEST_F(SettingsDlgViewModelTests, Can_test_valid_settings)
{
    // Arrange
    SettingsDlgViewModel sut;
    sut.setConnectCurrentDomain(false);
    sut.setConnectAsCurrentUser(true);
    sut.setAddress("LDAP://localhost:55555/CN=users,DC=testdata,DC=adbook,DC=local");

    // Act
    bool testResult = sut.testSettings();
    QString errorDescription = sut.testSettingsDetails();

    // Assert
    ASSERT_TRUE(testResult);
    ASSERT_TRUE(errorDescription.isEmpty());
}

TEST_F(SettingsDlgViewModelTests, Can_test_invalid_settings)
{
    // Arrange
    SettingsDlgViewModel sut;
    sut.setConnectCurrentDomain(false);
    sut.setConnectAsCurrentUser(true);
    sut.setAddress("LDAP://localhost:55555/CN=INVALID,DC=testdata,DC=adbook,DC=local");

    // Act
    bool testResult = sut.testSettings();
    QString errorDescription = sut.testSettingsDetails();

    // Assert
    ASSERT_FALSE(testResult);
    ASSERT_FALSE(errorDescription.isEmpty());
}

TEST_F(SettingsDlgViewModelTests, Must_emit_notification_signal_when_testSettingsDetails_are_changed)
{
    // Arrange
    SettingsDlgViewModel sut;
    sut.setConnectCurrentDomain(false);
    sut.setConnectAsCurrentUser(true);
    sut.setAddress("LDAP://localhost:55555/CN=users,DC=testdata,DC=adbook,DC=local");

    bool testSettingsDetailsChangedCalled = false;
    bool testSettingsDetailsChangedConnected = QObject::connect(
        &sut, &SettingsDlgViewModel::testSettingsDetailsChanged,
        [&testSettingsDetailsChangedCalled]() { testSettingsDetailsChangedCalled = true; }
    );

    // Act
    sut.testSettings();

    // Assert
    ASSERT_TRUE(testSettingsDetailsChangedConnected);
    ASSERT_TRUE(testSettingsDetailsChangedCalled);
}




