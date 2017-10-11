#include "pch.h"

// see the document 'How to setup the test environment on Windows 10.txt'

class AdPersonDescKeeperTests : public ::testing::Test {
public:
    void SetUp() override {
        CoInitialize(NULL);
    }
    void TearDown() override {
        CoUninitialize();
    }
};

TEST_F(AdPersonDescKeeperTests, CanSaveAndLoad) {
    std::shared_ptr<adbook::AbstractAdAccessFactory> factory =
        adbook::CreateAdAccessFactory(adbook::AbstractAdAccessFactory::ProductsType::Fake);
    // Act
    std::shared_ptr<adbook::AbstractAdPersonDescKeeper> keeper = factory->GetAdPersonDescKeeper();

    adbook::ConnectionParams connectionParams;
    connectionParams.ConnectDomainYouAreLoggedIn(false);
    connectionParams.UseCurrentUserCredentials(true);
    connectionParams.SetLdapPath(L"LDAP://localhost:55555/CN=users,DC=testdata,DC=adbook,DC=local");
    // the internal db file name depends on connection params
    keeper->SetNameByConnectionParams(connectionParams);

    using namespace adbook;
    std::vector<AdPersonDesc> adpList, adpList2;
    AdPersonDesc adp1, adp2, adp3;
    adp1.SetStringAttr(L"cn", L"user1");
    adp2.SetStringAttr(L"cn", L"user2");
    adp3.SetStringAttr(L"cn", L"user3");
    adpList.push_back(adp1);
    adpList.push_back(adp2);
    adpList.push_back(adp3);

    keeper->Save(adpList);
    adpList.clear();
    keeper->Load(adpList);
    // Assert
    ASSERT_TRUE(adpList.size() == 3);

    ASSERT_TRUE(adpList[0].GetStringAttr(L"cn") == L"user1");
    ASSERT_TRUE(adpList[1].GetStringAttr(L"cn") == L"user2");
    ASSERT_TRUE(adpList[2].GetStringAttr(L"cn") == L"user3");
}

