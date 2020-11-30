#include "pch.h"

TEST(AdPersonDescTests, Can_write_string_attribute) {
    // Arrange
    adbook::AdPersonDesc adp;
    // Act
    adp.SetStringAttr(L"cn", L"John Dow");
    // Assert
    ASSERT_TRUE(adp.IsAttributeSet(L"cn"));

}

TEST(AdPersonDescTests, Can_read_string_attribute) {
    // Arrange
    adbook::AdPersonDesc adp;

    // Act
    adp.SetStringAttr(L"cn", L"John Dow");

    // Assert
    ASSERT_TRUE(adp.GetStringAttr(L"cn") == L"John Dow");
    ASSERT_STREQ(adp.GetStringAttrPtr(L"cn"), L"John Dow");
}

TEST(AdPersonDescTests, Can_write_binary_attribute) {
    // Arrange
    adbook::AdPersonDesc adp;
    std::vector<byte> data = { 1,2,3 };

    // Act
    adp.SetBinaryAttr(L"thumbnailPhoto", data);

    // Assert
    ASSERT_TRUE(adp.IsAttributeSet(L"thumbnailPhoto"));
}

TEST(AdPersonDescTests, Can_read_binary_attribute) {
    // Arrange
    adbook::AdPersonDesc adp;
    std::vector<byte> data = { 1,2,3 };

    // Act
    adp.SetBinaryAttr(L"thumbnailPhoto", data);
    auto dataRead = adp.GetBinaryAttr(L"thumbnailPhoto");
    size_t numBytesRead = 0;
    auto dataReadPtr = adp.GetBinaryAttrPtr(L"thumbnailPhoto", numBytesRead);
    // Assert
    ASSERT_TRUE(dataRead == data);
    ASSERT_TRUE(data.size() == numBytesRead);
    ASSERT_TRUE(0 == memcmp(dataReadPtr, data.data(), numBytesRead));
}

TEST(AdPersonDescTests, Compairing_string_attributes) {
    // Arrange
    adbook::AdPersonDesc adp1, adp2;
    adp1.SetStringAttr(L"cn", L"John Dow");
    adp2.SetStringAttr(L"cn", L"Jane Dow");

    // Act
    bool compareResult1 = adp1.LexicographicalCompareStringAttrs(adp2, L"cn");
    // NOTE: the meaning of the return value is similar to https://en.cppreference.com/w/cpp/algorithm/lexicographical_compare
    bool compareResult2 = adp2.LexicographicalCompareStringAttrs(adp2, adbook::Attributes::CommonName);

    // Assert
    ASSERT_FALSE(compareResult1);
    ASSERT_FALSE(compareResult2);
}


TEST(AdPersonDescTests, Can_specify_writable_attributes)
{
    // Arrange
    adbook::AdPersonDesc adp;
    adbook::AdPersonDesc::AttrIds attrIds;
    attrIds.insert(adbook::Attributes::AttrId::CommonName);
    attrIds.insert(adbook::Attributes::AttrId::Company);
    attrIds.insert(adbook::Attributes::AttrId::Department);

    // Act
    adp.SetWritableAttributes(attrIds);

    // Assert
    ASSERT_TRUE(adp.IsAttributeWritable(adbook::Attributes::AttrId::CommonName));
    ASSERT_TRUE(adp.IsAttributeWritable(adbook::Attributes::AttrId::Company));
    ASSERT_TRUE(adp.IsAttributeWritable(adbook::Attributes::AttrId::Department));
    ASSERT_FALSE(adp.IsAttributeWritable(adbook::Attributes::AttrId::Email));
    ASSERT_FALSE(adp.IsAttributeWritable(adbook::Attributes::AttrId::GivenName));
    ASSERT_TRUE( adp.GetWritableAttributes() == attrIds);
}

TEST(AdPersonDescTests, Can_retrieve_dn_attr)
{
    // Arrange
    adbook::AdPersonDesc adp;
    adp.SetStringAttr(adbook::AdAttrDn, L"some_value");

    // Act
    auto dn = adp.GetDn();

    // Assert
    ASSERT_TRUE(dn == L"some_value");
}

