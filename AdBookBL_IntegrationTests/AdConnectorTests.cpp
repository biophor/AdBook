/*
Copyright (C) 2015-2021 Andrei Goncharov.

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

class AdConnectorTests : public TestSuiteBase {
public:
    void SetUp() override {
        CoInitialize(NULL);
    }
    void TearDown() override {
        CoUninitialize();
    }
};

TEST_F(AdConnectorTests, Can_connect_when_AdLdsInstance_is_accessible) {
    // Arrange
    adbook::AdConnector connector;
    adbook::ConnectionParams connectionParams;
    connectionParams.Set_ConnectDomainYouLoggedIn(false);
    connectionParams.Set_ConnectAsCurrentUser(true);
    connectionParams.SetAddress(GetLdapUrlForTestAdLdsInstance());

    // Act
    connector.Connect(connectionParams);

    // Assert
    ASSERT_TRUE(connector.IsConnected());
    connector.Disconnect();
    ASSERT_FALSE(connector.IsConnected());
}

TEST_F(AdConnectorTests, Cannot_connect_when_AdLdsInstance_is_inaccessible) {
    // Arrange
    adbook::AdConnector connector;
    adbook::ConnectionParams connectionParams;
    connectionParams.Set_ConnectDomainYouLoggedIn(false);
    connectionParams.Set_ConnectAsCurrentUser(true);
    std::wstring invalidLdapUrl = L"LDAP://localhost:44444/CN=users,DC=testdata,DC=adbook,DC=local";
    connectionParams.SetAddress(invalidLdapUrl);

    // Act & Assert
    ASSERT_THROW(connector.Connect(connectionParams), adbook::HrError);
}

TEST_F(AdConnectorTests, Can_download_when_string_attribute_name_is_valid) {
    // Arrange
    adbook::AdConnector connector;
    adbook::ConnectionParams connectionParams;
    connectionParams.Set_ConnectDomainYouLoggedIn(false);
    connectionParams.Set_ConnectAsCurrentUser(true);
    connectionParams.SetAddress(GetLdapUrlForTestAdLdsInstance());
    connector.Connect(connectionParams);

    // Act
    std::wstring cnAttrValue = connector.DownloadStringAttr(L"cn");

    // Assert
    ASSERT_TRUE(cnAttrValue == L"users");
}

TEST_F(AdConnectorTests, Cannot_download_when_string_attribute_name_is_invalid) {
    // Arrange
    adbook::AdConnector connector;
    adbook::ConnectionParams connectionParams;
    connectionParams.Set_ConnectDomainYouLoggedIn(false);
    connectionParams.Set_ConnectAsCurrentUser(true);
    connectionParams.SetAddress(GetLdapUrlForTestAdLdsInstance());
    connector.Connect(connectionParams);

    // Act & Assert
    ASSERT_THROW(connector.DownloadStringAttr(L"invalidname"), adbook::HrError);
}

TEST_F(AdConnectorTests, Download_must_return_empty_string_when_valid_string_attribute_has_no_value) {
    // Arrange
    adbook::AdConnector connector;
    adbook::ConnectionParams connectionParams;
    connectionParams.Set_ConnectDomainYouLoggedIn(false);
    connectionParams.Set_ConnectAsCurrentUser(true);
    connectionParams.SetAddress(GetLdapUrlForTestAdLdsInstance());
    connector.Connect(connectionParams);

    // Act
    std::wstring descriptionAttrValue = connector.DownloadStringAttr(L"description");

    // Assert
    ASSERT_TRUE(descriptionAttrValue.length() == 0);
}

TEST_F(AdConnectorTests, Can_upload_when_string_attribute_name_is_valid) {
    // Arrange
    adbook::AdConnector connector;
    adbook::ConnectionParams connectionParams;
    connectionParams.Set_ConnectDomainYouLoggedIn(false);
    connectionParams.Set_ConnectAsCurrentUser(true);
    connectionParams.SetAddress(GetLdapUrlForTestPersonOnAdLdsInstance());
    connector.Connect(connectionParams);

    // Act
    connector.UploadStringAttr(L"comment", L"");
    std::wstring emptyCommentValue = connector.DownloadStringAttr(L"comment");

    wchar_t commentValue[] = L"This is a comment";
    connector.UploadStringAttr(L"comment", commentValue);

    // Assert
    ASSERT_TRUE(emptyCommentValue.length() == 0);
    ASSERT_TRUE(connector.DownloadStringAttr(L"comment") == commentValue);
}

TEST_F(AdConnectorTests, Cannot_upload_when_string_attribute_name_is_invalid) {
    // Arrange
    adbook::AdConnector connector;
    adbook::ConnectionParams connectionParams;
    connectionParams.Set_ConnectDomainYouLoggedIn(false);
    connectionParams.Set_ConnectAsCurrentUser(true);
    connectionParams.SetAddress(GetLdapUrlForTestPersonOnAdLdsInstance());
    connector.Connect(connectionParams);

    // Act & Assert
    wchar_t commentValue[] = L"This is a comment";
    ASSERT_THROW(connector.UploadStringAttr(L"invalidname", commentValue), adbook::HrError);
}

TEST_F(AdConnectorTests, Can_download_when_binary_attribute_name_is_valid) {
    // Arrange
    adbook::AdConnector connector;
    adbook::ConnectionParams connectionParams;
    connectionParams.Set_ConnectDomainYouLoggedIn(false);
    connectionParams.Set_ConnectAsCurrentUser(true);
    connectionParams.SetAddress(GetLdapUrlForTestPersonOnAdLdsInstance());
    connector.Connect(connectionParams);

    // Act
    // this attribute contains predefined data. See the project 'AdBookBL_IntegrationTestsPrepareEnvironment'
    adbook::BinaryAttrVal value = connector.DownloadBinaryAttr(L"thumbnailPhoto");

    // Assert
    ASSERT_TRUE(value.size() == 1335);
    ASSERT_TRUE(value[0] == 0xFF);
    ASSERT_TRUE(value[1] == 0xD8);
    ASSERT_TRUE(value[2] == 0xFF);
}

TEST_F(AdConnectorTests, Cannot_download_when_binary_attribute_name_is_invalid) {
    // Arrange
    adbook::AdConnector connector;
    adbook::ConnectionParams connectionParams;
    connectionParams.Set_ConnectDomainYouLoggedIn(false);
    connectionParams.Set_ConnectAsCurrentUser(true);
    connectionParams.SetAddress(GetLdapUrlForTestPersonOnAdLdsInstance());
    connector.Connect(connectionParams);

    // Act & Assert
    // this attribute contains predefined data. See the project 'AdBookBL_IntegrationTestsPrepareEnvironment'
    ASSERT_THROW(connector.DownloadBinaryAttr(L"invalidname"), adbook::HrError);
}

TEST_F(AdConnectorTests, Can_upload_when_binary_attribute_name_is_valid) {
    // Arrange
    adbook::AdConnector connector;
    adbook::ConnectionParams connectionParams;
    connectionParams.Set_ConnectDomainYouLoggedIn(false);
    connectionParams.Set_ConnectAsCurrentUser(true);
    connectionParams.SetAddress(GetLdapUrlForTestPersonOnAdLdsInstance());
    connector.Connect(connectionParams);

    // Act
    connector.UploadBinaryAttr(L"photo", adbook::BinaryAttrVal{});
    adbook::BinaryAttrVal emptyTestData = connector.DownloadBinaryAttr(L"photo");
    adbook::BinaryAttrVal testData = {0x1,0x2,0x3};
    connector.UploadBinaryAttr(L"photo", testData);

    // Assert
    ASSERT_TRUE(connector.DownloadBinaryAttr(L"photo") == testData);
    ASSERT_TRUE(emptyTestData.empty());
}

TEST_F(AdConnectorTests, Cannot_upload_when_binary_attribute_name_is_invalid) {
    // Arrange
    adbook::AdConnector connector;
    adbook::ConnectionParams connectionParams;
    connectionParams.Set_ConnectDomainYouLoggedIn(false);
    connectionParams.Set_ConnectAsCurrentUser(true);
    connectionParams.SetAddress(GetLdapUrlForTestPersonOnAdLdsInstance());
    connector.Connect(connectionParams);

    // Act & Assert
    adbook::BinaryAttrVal testData = { 0x1,0x2,0x3 };
    ASSERT_THROW(connector.UploadBinaryAttr(L"invalidname", testData), adbook::HrError);
}

TEST_F(AdConnectorTests, Cant_rename_an_existing_object) {
    // Arrange
    adbook::AdConnector connector;
    adbook::ConnectionParams connectionParams;
    connectionParams.Set_ConnectDomainYouLoggedIn(false);
    connectionParams.Set_ConnectAsCurrentUser(true);
    connectionParams.SetAddress(GetLdapUrlForTestPersonOnAdLdsInstance());
    connector.Connect(connectionParams);

    // Act
    std::wstring currentCN = connector.DownloadStringAttr(L"cn");
    connector.Rename(L"Kristie Etherton XYZ");
    std::wstring newCN = connector.DownloadStringAttr(L"cn");
    connector.Rename(currentCN);

    // Assert
    ASSERT_TRUE(currentCN == L"Kristie Etherton");
    ASSERT_TRUE(newCN == L"Kristie Etherton XYZ");
}

