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

// see the document 'How to setup the test environment on Windows 10.txt'

class AdAccessFactoryTests : public ::testing::Test
{
public:
    void SetUp() override {
        CoInitialize(NULL);
    }
    void TearDown() override {
        CoUninitialize();
    }
};

//TEST_F(AdAccessFactoryTests, Creating_Factory) {
//    // Arrange & Act
//    std::shared_ptr<adbook::AbstractAdAccessFactory> factory =
//        adbook::CreateAdAccessFactory(adbook::AbstractAdAccessFactory::Type::Real);
//    // Assert
//    ASSERT_TRUE(factory != nullptr);
//    ASSERT_TRUE(factory->GetType() == adbook::AbstractAdAccessFactory::Type::Real);
//}
//
//TEST_F(AdAccessFactoryTests, Creating_Connector) {
//    // Arrange
//    std::shared_ptr<adbook::AbstractAdAccessFactory> factory =
//        adbook::CreateAdAccessFactory(adbook::AbstractAdAccessFactory::Type::Real);
//    // Act
//    std::unique_ptr<adbook::AbstractAdConnector> connector = factory->CreateConnector();
//    // Assert
//    ASSERT_TRUE(connector != nullptr);
//}
//
//TEST_F(AdAccessFactoryTests, Creating_Searcher) {
//    // Arrange
//    std::shared_ptr<adbook::AbstractAdAccessFactory> factory =
//        adbook::CreateAdAccessFactory(adbook::AbstractAdAccessFactory::Type::Real);
//    // Act
//    std::unique_ptr<adbook::AbstractAdSearcher> searcher = factory->CreateSearcher();
//    // Assert
//    ASSERT_TRUE(searcher != nullptr);
//}
//
//TEST_F(AdAccessFactoryTests, Getting_PersonDescKeeper) {
//    // Arrange
//    std::shared_ptr<adbook::AbstractAdAccessFactory> factory =
//        adbook::CreateAdAccessFactory(adbook::AbstractAdAccessFactory::Type::Real);
//    // Act
//    std::shared_ptr<adbook::AbstractAdPersonDescKeeper> keeper = factory->GetAdPersonDescKeeper();
//    // Assert
//    ASSERT_TRUE(keeper != nullptr);
//}


