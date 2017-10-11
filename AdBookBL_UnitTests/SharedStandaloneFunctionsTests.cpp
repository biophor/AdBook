#include "pch.h"

TEST(SharedStandaloneFunctionsTests, ExtractDirFromFilePath_Test) {
    // Arrange
    std::wstring filePath = L"c:/dir/file.txt";

    // Act
    std::wstring dirPath = adbook::ExtractDirFromFilePath(filePath);

    // Assert
    bool b = (dirPath == L"c:/dir") || (dirPath == L"c:/dir/") || (dirPath == L"c:\\dir\\") || (dirPath == L"c:\\dir");
    ASSERT_TRUE( b );
}


TEST(SharedStandaloneFunctionsTests, ToWcharBuf_Test) {
    // Arrange
    std::wstring data = L"hello world";

    // Act
    adbook::WcharBuf dataBuf = adbook::ToWcharBuf(data);

    // Assert
    ASSERT_TRUE(!dataBuf.empty());
    ASSERT_TRUE(dataBuf.size() == data.size() + 1);
    ASSERT_TRUE(0 == wcsncmp(dataBuf.data(), data.c_str(), data.length()));
}
