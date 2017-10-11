#include "pch.h"

TEST(LdapRequestBuilderTests, OneAttribute_ExactMatch_NoRules) {
    // Arrange
    using namespace adbook;
    LdapRequestBuilder builder;
    // Act
    builder.AddRule(L"cn", LdapRequestBuilder::ExactMatch, L"John Dow");
    std::wstring request = builder.Get();

    // Assert
    ASSERT_TRUE(request == L"(cn=John Dow)");
}

TEST(LdapRequestBuilderTests, OneAttribute_BeginWith_NoRules) {
    // Arrange
    using namespace adbook;
    LdapRequestBuilder builder;
    // Act
    builder.AddRule(L"cn", LdapRequestBuilder::BeginWith, L"John");
    std::wstring request = builder.Get();

    // Assert
    ASSERT_TRUE(request == L"(cn=John*)");
}

TEST(LdapRequestBuilderTests, OneAttribute_EndWith_NoRules) {
    // Arrange
    using namespace adbook;
    LdapRequestBuilder builder;
    // Act
    builder.AddRule(L"cn", LdapRequestBuilder::EndWith, L"Dow");
    std::wstring request = builder.Get();

    // Assert
    ASSERT_TRUE(request == L"(cn=*Dow)");
}

TEST(LdapRequestBuilderTests, OneAttribute_Contains_NoRules) {
    // Arrange
    using namespace adbook;
    LdapRequestBuilder builder;
    // Act
    builder.AddRule(L"cn", LdapRequestBuilder::Contains, L"Dow");
    std::wstring request = builder.Get();

    // Assert
    ASSERT_TRUE(request == L"(cn=*Dow*)");
}

TEST(LdapRequestBuilderTests, OneAttribute_ExactMatch_NotRule) {
    // Arrange
    using namespace adbook;
    LdapRequestBuilder builder;
    // Act
    builder.AddRule(L"cn", LdapRequestBuilder::ExactMatch, L"John Dow");
    builder.AddNOT();
    std::wstring request = builder.Get();

    // Assert
    ASSERT_TRUE(request == L"(!(cn=John Dow))");
}



TEST(LdapRequestBuilderTests, TwoAttributes_AndRule) {
    // Arrange
    using namespace adbook;
    LdapRequestBuilder builder;
    // Act
    builder.AddRule(L"cn", LdapRequestBuilder::ExactMatch, L"John Dow");
    builder.AddRule(L"company", LdapRequestBuilder::ExactMatch, L"MSFT");
    builder.AddAND();
    std::wstring request = builder.Get();

    // Assert
    ASSERT_TRUE(request == L"(&(cn=John Dow)(company=MSFT))");
}

TEST(LdapRequestBuilderTests, TwoAttributes_OrRule) {
    // Arrange
    using namespace adbook;
    LdapRequestBuilder builder;
    // Act
    builder.AddRule(L"cn", LdapRequestBuilder::ExactMatch, L"John Dow");
    builder.AddRule(L"cn", LdapRequestBuilder::ExactMatch, L"Jane Dow");
    builder.AddOR();

    std::wstring request = builder.Get();

    // Assert
    ASSERT_TRUE(request == L"(|(cn=John Dow)(cn=Jane Dow))");
}


TEST(LdapRequestBuilderTests, TwoAttributes_OrRule_NotRule) {
    // Arrange
    using namespace adbook;
    LdapRequestBuilder builder;
    // Act
    builder.AddRule(L"cn", LdapRequestBuilder::ExactMatch, L"John Dow");
    builder.AddRule(L"cn", LdapRequestBuilder::ExactMatch, L"Jane Dow");
    builder.AddOR();
    builder.AddNOT();
    std::wstring request = builder.Get();

    // Assert
    ASSERT_TRUE(request == L"(!(|(cn=John Dow)(cn=Jane Dow)))");
}

TEST(LdapRequestBuilderTests, ThreeAttributes_OrRule_AndRule) {
    // Arrange
    using namespace adbook;
    LdapRequestBuilder builder;
    // Act
    builder.AddRule(L"cn", LdapRequestBuilder::ExactMatch, L"John Dow");
    builder.AddRule(L"cn", LdapRequestBuilder::ExactMatch, L"Jane Dow");
    builder.AddOR();
    builder.AddRule(L"company", LdapRequestBuilder::ExactMatch, L"MSFT");
    builder.AddAND();
    std::wstring request = builder.Get();

    // Assert
    ASSERT_TRUE(request == L"(&(|(cn=John Dow)(cn=Jane Dow))(company=MSFT))");
}

