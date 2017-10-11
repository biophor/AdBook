#include "pch.h"

TEST(AdAccessFactoryTests, CanCreateFakeFactory) {
    // Arrange & Act
    std::shared_ptr<adbook::AbstractAdAccessFactory> factory =
        adbook::CreateAdAccessFactory(adbook::AbstractAdAccessFactory::ProductsType::Fake);
    // Assert
    ASSERT_TRUE(factory != nullptr);
    ASSERT_TRUE(factory->GetProductType() == adbook::AbstractAdAccessFactory::ProductsType::Fake);
}

TEST(AdAccessFactoryTests, CanCreateFakeConnector) {
    // Arrange
    std::shared_ptr<adbook::AbstractAdAccessFactory> factory =
        adbook::CreateAdAccessFactory(adbook::AbstractAdAccessFactory::ProductsType::Fake);
    // Act
    std::unique_ptr<adbook::AbstractAdConnector> connector = factory->CreateConnector();
    // Assert
    ASSERT_TRUE(connector != nullptr);
}

TEST(AdAccessFactoryTests, CanCreateFakeSearcher) {
    // Arrange
    std::shared_ptr<adbook::AbstractAdAccessFactory> factory =
        adbook::CreateAdAccessFactory(adbook::AbstractAdAccessFactory::ProductsType::Fake);
    // Act
    std::unique_ptr<adbook::AbstractAdSearcher> searcher = factory->CreateSearcher();
    // Assert
    ASSERT_TRUE(searcher != nullptr);
}

TEST(AdAccessFactoryTests, CanGetFakePersonDescKeeper) {
    // Arrange
    std::shared_ptr<adbook::AbstractAdAccessFactory> factory =
        adbook::CreateAdAccessFactory(adbook::AbstractAdAccessFactory::ProductsType::Fake);
    // Act
    std::shared_ptr<adbook::AbstractAdPersonDescKeeper> keeper = factory->GetAdPersonDescKeeper();
    // Assert
    ASSERT_TRUE(keeper != nullptr);
}

