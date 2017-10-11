#include "pch.h"

// see the document 'How to setup the test environment on Windows 10.txt'

class AdConnectorTests : public ::testing::Test {
public:
    void SetUp() override {
        CoInitialize(NULL);
    }
    void TearDown() override {
        CoUninitialize();
    }
};

TEST_F(AdConnectorTests, CanConnectToTheValidAdLdsInstance) {
    // Arrange
    std::shared_ptr<adbook::AbstractAdAccessFactory> factory =
        adbook::CreateAdAccessFactory(adbook::AbstractAdAccessFactory::ProductsType::Real);

    std::unique_ptr<adbook::AbstractAdConnector> connector =
        factory->CreateConnector();

    adbook::ConnectionParams connectionParams;
    connectionParams.ConnectDomainYouAreLoggedIn(false);
    connectionParams.UseCurrentUserCredentials(true);
    connectionParams.SetLdapPath(L"LDAP://localhost:55555/CN=users,DC=testdata,DC=adbook,DC=local");

    // Act
    connector->Connect(connectionParams);

    // Assert
    ASSERT_TRUE(connector->IsConnected());
    connector->Disconnect();
    ASSERT_FALSE(connector->IsConnected());
}

TEST_F(AdConnectorTests, CantConnectToTheInvalidAdLdsInstance) {
    // Arrange
    std::shared_ptr<adbook::AbstractAdAccessFactory> factory =
        adbook::CreateAdAccessFactory(adbook::AbstractAdAccessFactory::ProductsType::Real);

    std::unique_ptr<adbook::AbstractAdConnector> connector =
        factory->CreateConnector();

    adbook::ConnectionParams connectionParams;
    connectionParams.ConnectDomainYouAreLoggedIn(false);
    connectionParams.UseCurrentUserCredentials(true);
    connectionParams.SetLdapPath(L"LDAP://localhost:44444/CN=users,DC=testdata,DC=adbook,DC=local");

    // Act & Assert
    ASSERT_THROW(connector->Connect(connectionParams), adbook::HrError);
}


TEST_F(AdConnectorTests, CanDownloadValueOfExistingStringAttr) {
    // Arrange
    std::shared_ptr<adbook::AbstractAdAccessFactory> factory =
        adbook::CreateAdAccessFactory(adbook::AbstractAdAccessFactory::ProductsType::Real);

    std::unique_ptr<adbook::AbstractAdConnector> connector =
        factory->CreateConnector();

    adbook::ConnectionParams connectionParams;
    connectionParams.ConnectDomainYouAreLoggedIn(false);
    connectionParams.UseCurrentUserCredentials(true);
    connectionParams.SetLdapPath(L"LDAP://localhost:55555/CN=users,DC=testdata,DC=adbook,DC=local");
    connector->Connect(connectionParams);

    // Act
    std::wstring cnAttrValue = connector->DownloadStringAttr(L"cn");

    // Assert
    ASSERT_TRUE(cnAttrValue == L"users");
}

TEST_F(AdConnectorTests, CantDownloadValueOfNonExistingStringAttr) {
    // Arrange
    std::shared_ptr<adbook::AbstractAdAccessFactory> factory =
        adbook::CreateAdAccessFactory(adbook::AbstractAdAccessFactory::ProductsType::Real);

    std::unique_ptr<adbook::AbstractAdConnector> connector =
        factory->CreateConnector();

    adbook::ConnectionParams connectionParams;
    connectionParams.ConnectDomainYouAreLoggedIn(false);
    connectionParams.UseCurrentUserCredentials(true);
    connectionParams.SetLdapPath(L"LDAP://localhost:55555/CN=users,DC=testdata,DC=adbook,DC=local");
    connector->Connect(connectionParams);

    // Act & Assert
    ASSERT_THROW(connector->DownloadStringAttr(L"invvalidname"), adbook::HrError);
}


TEST_F(AdConnectorTests, CanUploadValueOfExistingStringAttr) {
    // Arrange
    std::shared_ptr<adbook::AbstractAdAccessFactory> factory =
        adbook::CreateAdAccessFactory(adbook::AbstractAdAccessFactory::ProductsType::Real);

    std::unique_ptr<adbook::AbstractAdConnector> connector =
        factory->CreateConnector();

    adbook::ConnectionParams connectionParams;
    connectionParams.ConnectDomainYouAreLoggedIn(false);
    connectionParams.UseCurrentUserCredentials(true);
    connectionParams.SetLdapPath(L"LDAP://localhost:55555/CN=Kristie Etherton,CN=users,DC=testdata,DC=adbook,DC=local");
    connector->Connect(connectionParams);

    // Act
    wchar_t commentValue[] = L"This is a comment";
    connector->UploadStringAttr(L"comment", commentValue);

    // Assert
    ASSERT_TRUE(connector->DownloadStringAttr(L"comment") == commentValue);
}

TEST_F(AdConnectorTests, CantUploadValueOfNonExistingStringAttr) {
    // Arrange
    std::shared_ptr<adbook::AbstractAdAccessFactory> factory =
        adbook::CreateAdAccessFactory(adbook::AbstractAdAccessFactory::ProductsType::Real);

    std::unique_ptr<adbook::AbstractAdConnector> connector =
        factory->CreateConnector();

    adbook::ConnectionParams connectionParams;
    connectionParams.ConnectDomainYouAreLoggedIn(false);
    connectionParams.UseCurrentUserCredentials(true);
    connectionParams.SetLdapPath(L"LDAP://localhost:55555/CN=Kristie Etherton,CN=users,DC=testdata,DC=adbook,DC=local");
    connector->Connect(connectionParams);

    // Act & Assert
    wchar_t commentValue[] = L"This is a comment";
    ASSERT_THROW(connector->UploadStringAttr(L"invalidname", commentValue), adbook::HrError);
}


TEST_F(AdConnectorTests, CanDownloadValueOfExistingBinaryAttr) {
    // Arrange
    std::shared_ptr<adbook::AbstractAdAccessFactory> factory =
        adbook::CreateAdAccessFactory(adbook::AbstractAdAccessFactory::ProductsType::Real);

    std::unique_ptr<adbook::AbstractAdConnector> connector =
        factory->CreateConnector();

    adbook::ConnectionParams connectionParams;
    connectionParams.ConnectDomainYouAreLoggedIn(false);
    connectionParams.UseCurrentUserCredentials(true);
    connectionParams.SetLdapPath(L"LDAP://localhost:55555/CN=Kristie Etherton,CN=users,DC=testdata,DC=adbook,DC=local");
    connector->Connect(connectionParams);

    // Act
    // this attribute contains predefined data. See the project 'AdBookBL_IntegrationTestsPrepareEnvironment'
    adbook::BinaryAttrVal value = connector->DownloadBinaryAttr(L"thumbnailPhoto");

    // Assert
    ASSERT_TRUE(value.size() == 1335);
    ASSERT_TRUE(value[0] == 0xFF);
    ASSERT_TRUE(value[1] == 0xD8);
    ASSERT_TRUE(value[2] == 0xFF);
}

TEST_F(AdConnectorTests, CantDownloadValueOfNonExistingBinaryAttr) {
    // Arrange
    std::shared_ptr<adbook::AbstractAdAccessFactory> factory =
        adbook::CreateAdAccessFactory(adbook::AbstractAdAccessFactory::ProductsType::Real);

    std::unique_ptr<adbook::AbstractAdConnector> connector =
        factory->CreateConnector();

    adbook::ConnectionParams connectionParams;
    connectionParams.ConnectDomainYouAreLoggedIn(false);
    connectionParams.UseCurrentUserCredentials(true);
    connectionParams.SetLdapPath(L"LDAP://localhost:55555/CN=Kristie Etherton,CN=users,DC=testdata,DC=adbook,DC=local");
    connector->Connect(connectionParams);

    // Act & Assert
    // this attribute contains predefined data. See the project 'AdBookBL_IntegrationTestsPrepareEnvironment'
    ASSERT_THROW(connector->DownloadBinaryAttr(L"invalidname"), adbook::HrError);
}

TEST_F(AdConnectorTests, CanUploadValueToExistingBinaryAttr) {
    // Arrange
    std::shared_ptr<adbook::AbstractAdAccessFactory> factory =
        adbook::CreateAdAccessFactory(adbook::AbstractAdAccessFactory::ProductsType::Real);

    std::unique_ptr<adbook::AbstractAdConnector> connector =
        factory->CreateConnector();

    adbook::ConnectionParams connectionParams;
    connectionParams.ConnectDomainYouAreLoggedIn(false);
    connectionParams.UseCurrentUserCredentials(true);
    connectionParams.SetLdapPath(L"LDAP://localhost:55555/CN=Kristie Etherton,CN=users,DC=testdata,DC=adbook,DC=local");
    connector->Connect(connectionParams);

    // Act
    adbook::BinaryAttrVal testData = {0x1,0x2,0x3};
    connector->UploadBinaryAttr(L"photo", testData);

    // Assert
    ASSERT_TRUE(connector->DownloadBinaryAttr(L"photo") == testData);
}


TEST_F(AdConnectorTests, CantUploadValueToNonExistingBinaryAttr) {
    // Arrange
    std::shared_ptr<adbook::AbstractAdAccessFactory> factory =
        adbook::CreateAdAccessFactory(adbook::AbstractAdAccessFactory::ProductsType::Real);

    std::unique_ptr<adbook::AbstractAdConnector> connector =
        factory->CreateConnector();

    adbook::ConnectionParams connectionParams;
    connectionParams.ConnectDomainYouAreLoggedIn(false);
    connectionParams.UseCurrentUserCredentials(true);
    connectionParams.SetLdapPath(L"LDAP://localhost:55555/CN=Kristie Etherton,CN=users,DC=testdata,DC=adbook,DC=local");
    connector->Connect(connectionParams);

    // Act & Assert
    adbook::BinaryAttrVal testData = { 0x1,0x2,0x3 };
    ASSERT_THROW(connector->UploadBinaryAttr(L"invalidname", testData), adbook::HrError);
}

