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

TEST_F(AdAccessFactoryTests, CanCreateRealFactory) {
    // Arrange & Act
    std::shared_ptr<adbook::AbstractAdAccessFactory> factory =
        adbook::CreateAdAccessFactory(adbook::AbstractAdAccessFactory::ProductsType::Real);
    // Assert
    ASSERT_TRUE(factory != nullptr);
    ASSERT_TRUE(factory->GetProductType() == adbook::AbstractAdAccessFactory::ProductsType::Real);
}

TEST_F(AdAccessFactoryTests, CanCreateRealConnector) {
    // Arrange
    std::shared_ptr<adbook::AbstractAdAccessFactory> factory =
        adbook::CreateAdAccessFactory(adbook::AbstractAdAccessFactory::ProductsType::Real);
    // Act
    std::unique_ptr<adbook::AbstractAdConnector> connector = factory->CreateConnector();
    // Assert
    ASSERT_TRUE(connector != nullptr);
}

TEST_F(AdAccessFactoryTests, CanCreateRealSearcher) {
    // Arrange
    std::shared_ptr<adbook::AbstractAdAccessFactory> factory =
        adbook::CreateAdAccessFactory(adbook::AbstractAdAccessFactory::ProductsType::Real);
    // Act
    std::unique_ptr<adbook::AbstractAdSearcher> searcher = factory->CreateSearcher();
    // Assert
    ASSERT_TRUE(searcher != nullptr);
}

TEST_F(AdAccessFactoryTests, CanGetRealPersonDescKeeper) {
    // Arrange
    std::shared_ptr<adbook::AbstractAdAccessFactory> factory =
        adbook::CreateAdAccessFactory(adbook::AbstractAdAccessFactory::ProductsType::Real);
    // Act
    std::shared_ptr<adbook::AbstractAdPersonDescKeeper> keeper = factory->GetAdPersonDescKeeper();
    // Assert
    ASSERT_TRUE(keeper != nullptr);
}


