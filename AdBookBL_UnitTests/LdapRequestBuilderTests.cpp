#include "pch.h"

using namespace adbook;

TEST(LdapRequestBuilderTests, OneAttribute_ExactMatch) {
    LdapRequestBuilder builder;

    builder.AddRule(L"cn", LdapRequestBuilder::ExactMatch, L"John Dow");
    std::wstring request = builder.Get();

    ASSERT_TRUE(request == L"(cn=John Dow)");
}

TEST(LdapRequestBuilderTests, OneAttribute_BeginWith) {
    LdapRequestBuilder builder;

    builder.AddRule(L"cn", LdapRequestBuilder::BeginWith, L"John");
    std::wstring request = builder.Get();

    ASSERT_TRUE(request == L"(cn=John*)");
}

TEST(LdapRequestBuilderTests, OneAttribute_EndWith) {
    LdapRequestBuilder builder;

    builder.AddRule(L"cn", LdapRequestBuilder::EndWith, L"Dow");
    std::wstring request = builder.Get();

    ASSERT_TRUE(request == L"(cn=*Dow)");
}

TEST(LdapRequestBuilderTests, OneAttribute_Contains) {
    LdapRequestBuilder builder;

    builder.AddRule(L"cn", LdapRequestBuilder::Contains, L"Dow");
    std::wstring request = builder.Get();

    ASSERT_TRUE(request == L"(cn=*Dow*)");
}

TEST(LdapRequestBuilderTests, OneAttribute_ExactMatch_NotRule) {
    LdapRequestBuilder builder;

    builder.AddRule(L"cn", LdapRequestBuilder::ExactMatch, L"John Dow");
    builder.AddNOT();
    std::wstring request = builder.Get();

    ASSERT_TRUE(request == L"(!(cn=John Dow))");
}

TEST(LdapRequestBuilderTests, TwoAttributes_AndRule) {
    LdapRequestBuilder builder;

    builder.AddRule(L"cn", LdapRequestBuilder::ExactMatch, L"John Dow");
    builder.AddRule(L"company", LdapRequestBuilder::ExactMatch, L"MSFT");
    builder.AddAND();
    std::wstring request = builder.Get();

    ASSERT_TRUE(request == L"(&(cn=John Dow)(company=MSFT))");
}

TEST(LdapRequestBuilderTests, TwoAttributes_OrRule) {
    LdapRequestBuilder builder;

    builder.AddRule(L"cn", LdapRequestBuilder::ExactMatch, L"John Dow");
    builder.AddRule(L"cn", LdapRequestBuilder::ExactMatch, L"Jane Dow");
    builder.AddOR();
    std::wstring request = builder.Get();

    ASSERT_TRUE(request == L"(|(cn=John Dow)(cn=Jane Dow))");
}

TEST(LdapRequestBuilderTests, TwoAttributes_OrRule_NotRule) {
    LdapRequestBuilder builder;

    builder.AddRule(L"cn", LdapRequestBuilder::ExactMatch, L"John Dow");
    builder.AddRule(L"cn", LdapRequestBuilder::ExactMatch, L"Jane Dow");
    builder.AddOR();
    builder.AddNOT();
    std::wstring request = builder.Get();

    ASSERT_TRUE(request == L"(!(|(cn=John Dow)(cn=Jane Dow)))");
}

TEST(LdapRequestBuilderTests, ThreeAttributes_OrRule_AndRule) {
    LdapRequestBuilder builder;

    builder.AddRule(L"cn", LdapRequestBuilder::ExactMatch, L"John Dow");
    builder.AddRule(L"cn", LdapRequestBuilder::ExactMatch, L"Jane Dow");
    builder.AddOR();
    builder.AddRule(L"company", LdapRequestBuilder::ExactMatch, L"MSFT");
    builder.AddAND();
    std::wstring request = builder.Get();

    ASSERT_TRUE(request == L"(&(|(cn=John Dow)(cn=Jane Dow))(company=MSFT))");
}

TEST(LdapRequestBuilderTests, Special_symbols_processing) {
    LdapRequestBuilder builder;

    builder.AddRule(L"cn", LdapRequestBuilder::ExactMatch, L"\\");
    builder.AddRule(L"cn", LdapRequestBuilder::ExactMatch, L"*");
    builder.AddRule(L"cn", LdapRequestBuilder::ExactMatch, L"/");
    builder.AddRule(L"cn", LdapRequestBuilder::ExactMatch, L"(");
    builder.AddRule(L"cn", LdapRequestBuilder::ExactMatch, L")");
    builder.AddOR();
    builder.AddObjectCategoryRule();
    builder.AddAND();
    std::wstring request = builder.Get();

    ASSERT_TRUE(request == L"(&(|(cn=\\5c)(cn=\\2a)(cn=\\2f)(cn=\\28)(cn=\\29))(objectCategory=person))");
}

