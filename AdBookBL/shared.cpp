#include "stdafx.h"
#include "error.h"
#include "shared.h"
#include "dllmain.h"
#include "resource.h"

namespace adbook
{

WcharBuf ToWcharBuf(const std::wstring & s)
{
    std::vector<wchar_t> buf(s.begin(), s.end());
    buf.push_back(0);   // trailing zero
    return buf;
}

std::wstring LoadString(UINT resId)
{
    wchar_t appName[256];
    if (!LoadStringW(moduleHandle, resId, appName, _countof(appName)))
    {
        throw HrError(HRESULT_FROM_WIN32(ERROR_RESOURCE_NOT_FOUND), __FUNCTIONW__);
    }
    return appName;
}

}   // namespace adbook
