/*
Copyright (C) 2015 Goncharov Andrei.

This file is part of the 'Active Directory Contact Book'.
'Active Directory Contact Book' is free software: you can redistribute it
and/or modify it under the terms of the GNU General Public License as published
by the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

'Active Directory Contact Book' is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
Public License for more details.

You should have received a copy of the GNU General Public License along with
'Active Directory Contact Book'. If not, see <http://www.gnu.org/licenses/>.
*/

#include "stdafx.h"
#include "error.h"

namespace adbook
{

// the following code based on the example https://msdn.microsoft.com/en-us/library/aa772347%28v=vs.85%29.aspx

struct ADSERRMSG
{
    HRESULT hr;
    wchar_t * errorName;
};

#define ADDADSERROR(x)   x, L#x

ADSERRMSG adsErr[] =
{
    ADDADSERROR(E_ADS_BAD_PATHNAME),
    ADDADSERROR(E_ADS_INVALID_DOMAIN_OBJECT),
    ADDADSERROR(E_ADS_INVALID_USER_OBJECT),
    ADDADSERROR(E_ADS_INVALID_COMPUTER_OBJECT),
    ADDADSERROR(E_ADS_UNKNOWN_OBJECT),
    ADDADSERROR(E_ADS_PROPERTY_NOT_SET),
    ADDADSERROR(E_ADS_PROPERTY_NOT_SUPPORTED),
    ADDADSERROR(E_ADS_PROPERTY_INVALID),
    ADDADSERROR(E_ADS_BAD_PARAMETER),
    ADDADSERROR(E_ADS_OBJECT_UNBOUND),
    ADDADSERROR(E_ADS_PROPERTY_NOT_MODIFIED),
    ADDADSERROR(E_ADS_PROPERTY_MODIFIED),
    ADDADSERROR(E_ADS_CANT_CONVERT_DATATYPE),
    ADDADSERROR(E_ADS_PROPERTY_NOT_FOUND),
    ADDADSERROR(E_ADS_OBJECT_EXISTS),
    ADDADSERROR(E_ADS_SCHEMA_VIOLATION),
    ADDADSERROR(E_ADS_COLUMN_NOT_SET),
    ADDADSERROR(E_ADS_INVALID_FILTER)
};
ADSERRMSG * adsErrEnd = adsErr + _countof(adsErr);

bool IsAdsiSpecificCode(const HRESULT hr)
{
    return (hr > 0x80005000 && hr < 0x80006000);
}

std::wstring GetAdsiErrorName(const HRESULT hr)
{
    const ADSERRMSG * errItem = std::find_if(adsErr, adsErrEnd, [hr](const ADSERRMSG & i) { return (i.hr == hr); });
    if (errItem == adsErrEnd)
    {
        return std::wstring();
    }
    return errItem->errorName;
}

std::wstring GetAdsiExtErrorDesc()
{
    wchar_t lastAdsiErrorBuffer[512] = L"";
    wchar_t lastAdsiErrorName[512] = L"";
    DWORD   lastAdsiError = 0;
    std::wstring errorDesc;

    HRESULT hr = ADsGetLastError(&lastAdsiError, lastAdsiErrorBuffer, _countof(lastAdsiErrorBuffer), lastAdsiErrorName,
        _countof(lastAdsiErrorName));

    if (SUCCEEDED(hr) && lastAdsiError != ERROR_INVALID_DATA && wcslen(lastAdsiErrorBuffer) && wcslen(lastAdsiErrorName))
    {
        errorDesc = L"ADSI Extended Error: ";
        errorDesc += lastAdsiErrorName;
        errorDesc += L" : ";
        errorDesc += lastAdsiErrorBuffer;
    }
    return errorDesc;
}

std::wstring GetWin32ErrorDesc(const DWORD lastError)
{
    std::wstring errorDesc;
    LPWSTR buf = nullptr;
    FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, nullptr,
        lastError, 0, reinterpret_cast<LPWSTR>(&buf), 0, nullptr);
    if (nullptr != buf)
    {
        errorDesc = buf;
        LocalFree(buf);
    }
    return errorDesc;
}

std::wstring GetHrErrorDesc(const HRESULT hr)
{
    std::wstring errorDesc;
    if (SUCCEEDED(hr))
    {
        return L"Success";
    }
    if (IsAdsiSpecificCode(hr))
    {
        errorDesc = GetAdsiErrorName(hr);
    }
    else if (HRESULT_FACILITY(hr) == FACILITY_WIN32)
    {
        errorDesc = GetWin32ErrorDesc(boost::numeric_cast<DWORD>(hr));
    }
    std::wstring adsiExtErr = GetAdsiExtErrorDesc();
    if (!adsiExtErr.empty())
    {
        errorDesc += L". ";
        errorDesc += adsiExtErr;
    }
    return errorDesc;
}

std::wstring HrError::What() const
{
    return GetHrErrorDesc(hr_);
}

}   // namespace adbook
