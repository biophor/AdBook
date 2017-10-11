#include "pch.h"

TEST(SharedStandaloneFunctionsTests, LoadStringW_Test) {
    std::wstring appTitle =  adbook::LoadStringW(AFX_IDS_APP_TITLE);
    ASSERT_TRUE(!appTitle.empty());
}