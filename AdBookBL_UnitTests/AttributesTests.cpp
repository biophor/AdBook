#include "pch.h"

TEST(AttributesTests, CanProvideTheListOfSupportedAttributes) {
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

TEST(AttributesTests, CanConvertBetweenAttrIdAndAttrName) {
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

TEST(AttributesTests, IsAttibuteSupportedMustReturnTrueForSupportedAttributes) {
    // Arrange
    const auto & attrs = adbook::Attributes::GetInstance();
    std::vector<adbook::Attributes::AttrId> attrIds = attrs.GetAttrIds();
    // Act & Assert
    for (auto attrId : attrIds) {
        const std::wstring attrName = attrs.GetLdapAttrName(attrId);
        ASSERT_TRUE(attrs.IsAttrSupported(attrName.c_str()));
        ASSERT_TRUE(attrs.IsAttrSupported(attrId));
    }
}

TEST(AttributesTests, IsAttibuteSupportedMustReturnFalseForUnsupportedAttributes) {
    // Arrange
    const auto & attrs = adbook::Attributes::GetInstance();
    std::vector<adbook::Attributes::AttrId> attrIds = attrs.GetAttrIds();
    // Act & Assert
    ASSERT_FALSE(attrs.IsAttrSupported(L"InvalidAttrName"));
    ASSERT_FALSE(attrs.IsAttrSupported(static_cast<adbook::Attributes::AttrId>(99)));
}

//
TEST(AttributesTests, GetAdsiComplientAttrNamesMethodTest) {
    // Arrange
    const auto & attrs = adbook::Attributes::GetInstance();
    std::vector<adbook::Attributes::AttrId> attrIds = attrs.GetAttrIds();
    std::vector<adbook::WcharBuf> attrNames = attrs.GetAdsiComplientAttrNames();
    // Act & Assert
    for (auto && attrName : attrNames) {
        ASSERT_TRUE(attrs.IsAttrSupported(attrName.data()));
    }
}

TEST(AttributesTests, IsStringMethodTest) {
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

TEST(AttributesTests, IsEditableMethodTest) {
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

TEST(AttributesTests, IsEditableStringMethodTest) {
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

TEST(AttributesTests, GetAttrOidMustReturnOidLikeString) {
    // Arrange
    const auto & attrs = adbook::Attributes::GetInstance();
    std::vector<adbook::Attributes::AttrId> attrIds = attrs.GetAttrIds();

    // Act & Assert
    for (auto attrId : attrIds) {
        std::wstring oid = attrs.GetAttrOid(attrId);
        ASSERT_TRUE(oid.find_first_not_of(L"0123456789.") == std::wstring::npos);
    }
}


TEST(AttributesTests, GetUiAttrNameMustReturnNonEmptyString) {
    // Arrange
    const auto & attrs = adbook::Attributes::GetInstance();
    std::vector<adbook::Attributes::AttrId> attrIds = attrs.GetAttrIds();

    // Act & Assert
    for (auto attrId : attrIds) {
        std::wstring uiName = attrs.GetUiAttrName(attrId);
        ASSERT_TRUE(!uiName.empty());
    }
}

TEST(AttributesTests, GetAttrMaxLengthMustReturnNonZeroSize) {
    // Arrange
    const auto & attrs = adbook::Attributes::GetInstance();
    std::vector<adbook::Attributes::AttrId> attrIds = attrs.GetAttrIds();

    // Act & Assert
    for (auto attrId : attrIds) {
        size_t maxLength = attrs.GetAttrMaxLength(attrId);
        ASSERT_TRUE(maxLength > 0);
    }
}


TEST(AttributesTests, GetBinaryAttrMaxLengthMustReturnNonZeroSize) {
    // Arrange
    const auto & attrs = adbook::Attributes::GetInstance();

    // Act & Assert
    size_t maxLength = attrs.GetBinaryAttrMaxLength();
    ASSERT_TRUE(maxLength > 0);
}

TEST(AttributesTests, GetTextAttrMaxLengthMustReturnNonZeroSize) {
    // Arrange
    const auto & attrs = adbook::Attributes::GetInstance();

    // Act & Assert
    size_t maxLength = attrs.GetTextAttrMaxLength();
    ASSERT_TRUE(maxLength > 0);
}


TEST(AttributesTests, GetLdapAttrNamePtrMustReturnPointerToAttrName) {
    // Arrange
    const auto & attrs = adbook::Attributes::GetInstance();
    std::vector<adbook::Attributes::AttrId> attrIds = attrs.GetAttrIds();

    // Act & Assert
    for (auto attrId : attrIds) {
        std::wstring ldapName = attrs.GetLdapAttrName(attrId);
        const wchar_t * ldapNamePtr = attrs.GetLdapAttrNamePtr(attrId);
        ASSERT_TRUE(ldapNamePtr != nullptr);
        ASSERT_TRUE(ldapName == ldapNamePtr);
    }
}

