#include "pch.h"

//TEST(AdAccessFactoryTests, Can_create_Factory) {
//    // Arrange & Act
//    std::shared_ptr<adbook::AbstractAdAccessFactory> unitTestFactory =
//        adbook::CreateAdAccessFactory(adbook::AbstractAdAccessFactory::Type::Test);
//    // Assert
//    ASSERT_TRUE(unitTestFactory != nullptr);
//    ASSERT_TRUE(unitTestFactory->GetType() == adbook::AbstractAdAccessFactory::Type::Test);
//}
//
//TEST(AdAccessFactoryTests, Can_create_Connector) {
//    // Arrange
//    std::shared_ptr<adbook::AbstractAdAccessFactory> unitTestFactory =
//        adbook::CreateAdAccessFactory(adbook::AbstractAdAccessFactory::Type::Test);
//    // Act
//    std::unique_ptr<adbook::AbstractAdConnector> connector = unitTestFactory->CreateConnector();
//    // Assert
//    ASSERT_TRUE(connector != nullptr);
//}
//
//TEST(AdAccessFactoryTests, Can_create_Searcher) {
//    // Arrange
//    std::shared_ptr<adbook::AbstractAdAccessFactory> unitTestFactory =
//        adbook::CreateAdAccessFactory(adbook::AbstractAdAccessFactory::Type::Test);
//    // Act
//    std::unique_ptr<adbook::AbstractAdSearcher> searcher = unitTestFactory->CreateSearcher();
//    // Assert
//    ASSERT_TRUE(searcher != nullptr);
//}
//
//TEST(AdAccessFactoryTests, Can_get_PersonDescKeeper) {
//    // Arrange
//    std::shared_ptr<adbook::AbstractAdAccessFactory> unitTestFactory =
//        adbook::CreateAdAccessFactory(adbook::AbstractAdAccessFactory::Type::Test);
//    // Act
//    std::shared_ptr<adbook::AbstractAdPersonDescKeeper> keeper = unitTestFactory->GetAdPersonDescKeeper();
//    // Assert
//    ASSERT_TRUE(keeper != nullptr);
//}
//
