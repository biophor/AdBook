#include "pch.h"

TEST(AttributesTests, Can_provide_the_list_of_supported_attributes)
{
    // Arrange
    const auto & attrs = adbook::Attributes::GetInstance();
    size_t numAttrs = attrs.GetAttrCount();
    // Act
    std::vector<adbook::Attributes::AttrId> attrIds = attrs.GetAttrIds();
    // Assert
    ASSERT_TRUE(numAttrs == 16);
    ASSERT_TRUE(std::find(attrIds.begin(), attrIds.end(), adbook::Attributes::CommonName) != attrIds.end());
    ASSERT_TRUE(std::find(attrIds.begin(), attrIds.end(), adbook::Attributes::Email) != attrIds.end());
    ASSERT_TRUE(std::find(attrIds.begin(), attrIds.end(), adbook::Attributes::Company) != attrIds.end());
    ASSERT_TRUE(std::find(attrIds.begin(), attrIds.end(), adbook::Attributes::Title) != attrIds.end());
    ASSERT_TRUE(std::find(attrIds.begin(), attrIds.end(), adbook::Attributes::Department) != attrIds.end());
    ASSERT_TRUE(std::find(attrIds.begin(), attrIds.end(), adbook::Attributes::State) != attrIds.end());
    ASSERT_TRUE(std::find(attrIds.begin(), attrIds.end(), adbook::Attributes::EmpId) != attrIds.end());
    ASSERT_TRUE(std::find(attrIds.begin(), attrIds.end(), adbook::Attributes::GivenName) != attrIds.end());
    ASSERT_TRUE(std::find(attrIds.begin(), attrIds.end(), adbook::Attributes::SurName) != attrIds.end());
    ASSERT_TRUE(std::find(attrIds.begin(), attrIds.end(), adbook::Attributes::WorkPhone) != attrIds.end());
    ASSERT_TRUE(std::find(attrIds.begin(), attrIds.end(), adbook::Attributes::DisplayName) != attrIds.end());
    ASSERT_TRUE(std::find(attrIds.begin(), attrIds.end(), adbook::Attributes::Locality) != attrIds.end());
    ASSERT_TRUE(std::find(attrIds.begin(), attrIds.end(), adbook::Attributes::ThumbnailPhoto) != attrIds.end());
    ASSERT_TRUE(std::find(attrIds.begin(), attrIds.end(), adbook::Attributes::Dn) != attrIds.end());
}

TEST(AttributesTests, Can_convert_between_AttrId_and_ldap_attr_name)
{
    // Arrange
    const auto & attrs = adbook::Attributes::GetInstance();
    std::vector<adbook::Attributes::AttrId> attrIds = attrs.GetAttrIds();
    // Act & Assert
    for (auto attrId : attrIds) {
        const std::wstring attrName = attrs.GetLdapAttrName(attrId);
        ASSERT_TRUE(!attrName.empty());
        const auto attrIdCopy = attrs.GetAttrId(attrName.c_str());
        ASSERT_TRUE(attrIdCopy == attrId);
    }
}

TEST(AttributesTests, Provide_information_on_supported_attributes)
{
    // Arrange
    const auto & attrs = adbook::Attributes::GetInstance();
    std::vector<adbook::Attributes::AttrId> attrIds = attrs.GetAttrIds();
    // Act & Assert
    for (auto attrId : attrIds) {
        const std::wstring attrName = attrs.GetLdapAttrName(attrId);
        ASSERT_TRUE(attrs.IsAttrSupported(attrName.c_str()));
        ASSERT_TRUE(attrs.IsAttrSupported(attrId));
    }
    // Assert
    ASSERT_FALSE(attrs.IsAttrSupported(L"InvalidAttrName"));
    ASSERT_FALSE(attrs.IsAttrSupported(static_cast<adbook::Attributes::AttrId>(99)));
}

TEST(AttributesTests, Provides_information_on_attribute_type)
{
    // Arrange
    const auto & attrs = adbook::Attributes::GetInstance();
    std::vector<adbook::Attributes::AttrId> attrIds = attrs.GetAttrIds();

    // Act & Assert
    for (auto attrId : attrIds) {
        if (attrId ==adbook::Attributes::AttrId::ThumbnailPhoto) {
            ASSERT_FALSE(attrs.IsString(attrId));
        }
        else {
            ASSERT_TRUE(attrs.IsString(attrId));
        }
    }
}

TEST(AttributesTests, Provides_information_on_attributes_which_can_be_changed_directly)
{
    // Arrange
    const auto & attrs = adbook::Attributes::GetInstance();
    std::vector<adbook::Attributes::AttrId> attrIds = attrs.GetAttrIds();

    // Act & Assert
    for (auto attrId : attrIds) {
        if (attrId == adbook::Attributes::AttrId::Dn) {
            ASSERT_FALSE(attrs.IsEditable(attrId));
        }
        else {
            ASSERT_TRUE(attrs.IsEditable(attrId));
        }
    }
}

TEST(AttributesTests, Provides_information_on_string_attrs_which_can_be_changed_directly)
{
    // Arrange
    const auto & attrs = adbook::Attributes::GetInstance();
    std::vector<adbook::Attributes::AttrId> attrIds = attrs.GetAttrIds();

    // Act & Assert
    for (auto attrId : attrIds) {
        if (attrId == adbook::Attributes::AttrId::Dn || attrId == adbook::Attributes::AttrId::ThumbnailPhoto) {
            ASSERT_FALSE(attrs.IsEditableString(attrId));
        }
        else {
            ASSERT_TRUE(attrs.IsEditableString(attrId));
        }
    }
}

TEST(AttributesTests, Provides_information_on_attribute_oids)
{
    // Arrange
    const auto & attrs = adbook::Attributes::GetInstance();
    std::vector<adbook::Attributes::AttrId> attrIds = attrs.GetAttrIds();

    // Act & Assert
    for (auto attrId : attrIds) {
        std::wstring oid = attrs.GetAttrOid(attrId);
        ASSERT_TRUE(oid.find_first_not_of(L"0123456789.") == std::wstring::npos);
    }
}

TEST(AttributesTests, Provides_user_friendly_names_on_attributes) {
    // Arrange
    const auto & attrs = adbook::Attributes::GetInstance();
    std::vector<adbook::Attributes::AttrId> attrIds = attrs.GetAttrIds();

    // Act & Assert
    for (auto attrId : attrIds) {
        std::wstring uiName = attrs.GetUiAttrName(attrId);
        ASSERT_TRUE(!uiName.empty());
    }
}

