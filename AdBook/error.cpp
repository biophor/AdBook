#include "stdafx.h"
#include "error.h"

namespace
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
	return !!(hr & 0x00005000);
}

CString GetAdsiErrorName(const HRESULT hr)
{
	const ADSERRMSG * errItem = std::find_if(adsErr, adsErrEnd, [hr](const ADSERRMSG & i) { return (i.hr == hr); });
	if (errItem == adsErrEnd)
	{
		return CString();
	}
	return errItem->errorName;
}

CString GetAdsiExtErrorDesc()
{
	wchar_t lastAdsiErrorBuffer[512] = L"";
	wchar_t lastAdsiErrorName[512] = L"";
	DWORD   lastAdsiError = 0;
    CString errorDesc;
	
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

CString GetWin32ErrorDesc(const DWORD lastError)
{
    CString errorDesc;
	LPWSTR buf = NULL;
	FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL,
		lastError, 0, reinterpret_cast<LPWSTR>(&buf), 0, NULL);
	if (NULL != buf)
	{
		errorDesc = buf;
		LocalFree(buf);
	}
	return errorDesc;
}

CString GetHrErrorDesc(const HRESULT hr)
{
    CString errorDesc;
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
		errorDesc = GetWin32ErrorDesc(static_cast<DWORD>(hr));
	}
    CString adsiExtErr = GetAdsiExtErrorDesc();
	if (!adsiExtErr.IsEmpty())
	{
		errorDesc += L". ";
		errorDesc += adsiExtErr;
	}
	return errorDesc;
}


}// anon namespace


CString HrError::What() const
{
    return GetHrErrorDesc(hr_);
}

CString Win32Error::What() const
{
    return GetWin32ErrorDesc(dw_);
}
