#pragma once

namespace adbook
{

typedef std::vector<wchar_t> WcharBuf;  // Some ADSI functions require non-const IN arguments.    

WcharBuf ADBOOKBL_API ToWcharBuf(const std::wstring & s);

std::wstring LoadString(UINT resId);

}   // namespace adbook
