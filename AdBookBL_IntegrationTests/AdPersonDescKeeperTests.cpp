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

using namespace adbook;

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

std::vector<AdPersonDesc> CreateAdPersonList(size_t num = 8)
{
    std::vector<AdPersonDesc> ret;
    for (size_t i = 0; i < num; ++i) {
        AdPersonDesc adp;
        auto & attrTraits = Attributes::GetInstance();
        auto attrIds = attrTraits.GetAttrIds();
        for (auto id : attrIds) {
            if (id == Attributes::CommonName) {
                adp.SetStringAttr(L"cn", std::to_wstring(i));
                continue;
            }
            if (attrTraits.IsString(id)) {
                const std::wstring stringAttrValue = std::wstring(L"attr value ") + std::to_wstring(id);
                adp.SetStringAttr(attrTraits.GetLdapAttrNamePtr(id), stringAttrValue);
            }
            else {
                BinaryAttrVal binaryAttrValue{ id };
                adp.SetBinaryAttr(attrTraits.GetLdapAttrNamePtr(id), binaryAttrValue);
            }
        }
        ret.push_back(adp);
    }
    return ret;
}

TEST_F(AdPersonDescKeeperTests, Can_save_and_load)
{
    // Arrange
    std::shared_ptr<adbook::AbstractAdAccessFactory> factory = adbook::GetAdAccessFactory();
    std::shared_ptr<adbook::AbstractAdPersonDescKeeper> keeper = factory->GetAdPersonDescKeeper();
    adbook::ConnectionParams connectionParams;
    connectionParams.Set_ConnectDomainYouLoggedIn(false);
    connectionParams.Set_ConnectAsCurrentUser(true);
    connectionParams.SetAddress(L"LDAP://localhost:55555/CN=fake_users,DC=testdata,DC=adbook,DC=local");
    // the internal db file name depends on connection params
    keeper->SetNameByConnectionParams(connectionParams);

    // Act
    std::vector<AdPersonDesc> emptyList;
    keeper->Save(emptyList);
    keeper->Load(emptyList);

    std::vector<AdPersonDesc> adpList = CreateAdPersonList();
    std::vector<AdPersonDesc> adpListCopy = adpList;

    keeper->Save(adpList);
    adpList.clear();
    keeper->Load(adpList);

    // Assert
    ASSERT_TRUE(emptyList.size() == 0);
    ASSERT_TRUE(adpList == adpListCopy);
}

