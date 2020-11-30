#include "pch.h"

TEST(SharedStandaloneFunctionsTests, ExtractDirFromFilePath_Test)
{
    // Arrange
    std::wstring filePath = L"c:/dir/file.txt";

    // Act
    std::wstring dirPath = adbook::ExtractDirFromFilePath(filePath);

    // Assert
    bool b = (dirPath == L"c:/dir") || (dirPath == L"c:/dir/") || (dirPath == L"c:\\dir\\") || (dirPath == L"c:\\dir");
    ASSERT_TRUE( b );
}

TEST(SharedStandaloneFunctionsTests, ToWcharBuf_Test)
{
    // Arrange
    std::wstring data = L"hello world";

    // Act
    adbook::WcharBuf dataBuf = adbook::ToWcharBuf(data);

    // Assert
    ASSERT_TRUE(!dataBuf.empty());
    ASSERT_TRUE(dataBuf.size() == data.size() + 1);
    ASSERT_TRUE(0 == wcsncmp(dataBuf.data(), data.c_str(), data.length()));
}

TEST(SharedStandaloneFunctionsTests, Trim_EmptyString)
{
    // Arrange
    std::wstring originalString = L"";

    // Act
    std::wstring trimmedString = adbook::Trim(originalString);

    // Assert
    ASSERT_TRUE(trimmedString == std::wstring());
}

TEST(SharedStandaloneFunctionsTests, Trim_OnlySpaces)
{
    // Arrange
    std::wstring originalString = L" ";
    std::wstring originalString2 = L"     ";
    std::wstring originalString3 = L"\t \n \r";
    std::wstring originalString4 = L" \t\n\r ";

    // Act
    std::wstring trimmedString = adbook::Trim(originalString);
    std::wstring trimmedString2 = adbook::Trim(originalString2);
    std::wstring trimmedString3 = adbook::Trim(originalString3);
    std::wstring trimmedString4 = adbook::Trim(originalString4);

    // Assert
    ASSERT_TRUE(trimmedString.empty());
    ASSERT_TRUE(trimmedString2.empty());
    ASSERT_TRUE(trimmedString3.empty());
    ASSERT_TRUE(trimmedString4.empty());
}

TEST(SharedStandaloneFunctionsTests, Trim_SpacesAndNonSpaces)
{
    // Arrange
    std::wstring originalString = L" z";
    std::wstring originalString2 = L"  z   ";
    std::wstring originalString3 = L"\tZ\nZ\r";
    std::wstring originalString4 = L" ZZ\t\n\r ";

    // Act
    std::wstring trimmedString = adbook::Trim(originalString);
    std::wstring trimmedString2 = adbook::Trim(originalString2);
    std::wstring trimmedString3 = adbook::Trim(originalString3);
    std::wstring trimmedString4 = adbook::Trim(originalString4);

    // Assert
    ASSERT_TRUE(trimmedString == L"z");
    ASSERT_TRUE(trimmedString2 == L"z");
    ASSERT_TRUE(trimmedString3 == L"Z\nZ");
    ASSERT_TRUE(trimmedString4 == L"ZZ");
}


TEST(SharedStandaloneFunctionsTests, ReplaceAll_T1)
{
    // Arrange
    std::wstring sut = L"aaa";
    // Act
    auto sut2 = adbook::ReplaceAll(sut, L"a", L"aa");
    // Assert
    ASSERT_EQ(sut2, L"aaaaaa");
}

TEST(SharedStandaloneFunctionsTests, ReplaceAll_T2)
{
    // Arrange
    std::wstring sut = L"aaa";
    // Act
    auto sut2 = adbook::ReplaceAll(sut, L"a", L"b");
    // Assert
    ASSERT_EQ(sut2, L"bbb");
}


TEST(SharedStandaloneFunctionsTests, ReplaceAllInPlace_T1)
{
    // Arrange
    std::wstring sut = L"aaa";
    // Act
    adbook::ReplaceAllInPlace(sut, L"a", L"aa");
    // Assert
    ASSERT_EQ(sut, L"aaaaaa");
}

TEST(SharedStandaloneFunctionsTests, ReplaceAllInPlace_T2)
{
    // Arrange
    std::wstring sut = L"aaa";
    // Act
    adbook::ReplaceAllInPlace(sut, L"a", L"b");
    // Assert
    ASSERT_EQ(sut, L"bbb");
}

TEST(SharedStandaloneFunctionsTests, ReplaceAllInPlace_T3)
{
    // Arrange
    std::wstring sut = L"aaaaaa";
    // Act
    adbook::ReplaceAllInPlace(sut, L"aa", L"a");
    // Assert
    ASSERT_EQ(sut, L"aaa");
}

TEST(SharedStandaloneFunctionsTests, ReplaceAllInPlace_T4)
{
    // Arrange
    std::wstring sut = L"aaaaaa";
    // Act
    adbook::ReplaceAllInPlace(sut, L"a", L"");
    // Assert
    ASSERT_EQ(sut, L"");
}

TEST(SharedStandaloneFunctionsTests, ToLower_T1)
{
    // Arrange
    std::wstring sut = L"AAA";
    // Act
    sut = adbook::ToLower(sut);
    // Assert
    ASSERT_EQ(sut, L"aaa");
}

