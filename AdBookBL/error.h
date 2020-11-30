/*
Copyright (C) 2015-2021 Andrei Goncharov.

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

#pragma once
#include "AdBookBLExport.h"
#include "debug.h"

namespace adbook
{

class ADBOOKBL_API Error : public std::exception
{
public:
    Error(const wchar_t * what, const wchar_t * where)
    {
        if (what) {
            size_t numCharsConverted = 0;
            wcstombs_s(&numCharsConverted, _stdExceptionWhatBuf, what, _TRUNCATE);
            wcsncpy_s(_whatBuf, what, _TRUNCATE);
        }
        if (where) {
            wcsncpy_s(_whereBuf, where, _TRUNCATE);
        }
    }

    Error(const wchar_t * where) {
        if (where) {
            wcscpy_s(_whereBuf, where);
        }
    }

    const wchar_t * What() const {
        return _whatBuf;
    }

    const wchar_t * Where() const {
        return _whereBuf;
    }

    virtual const char * what() const {
        return _stdExceptionWhatBuf;
    }

protected:
    char _stdExceptionWhatBuf[128] = {};    // for 'const char * std::exception::what()'
    wchar_t _whatBuf[128] = {};
    wchar_t _whereBuf[64] = {};
};

#define MY_TRACE_ERROR(e) MY_TRACE(L"Error: what:%s where:%s", e.What(), e.Where());

class ADBOOKBL_API HrError : public Error
{
public:
    HrError(const HRESULT hr, const wchar_t * where = nullptr)
        : Error(where), _hr(hr)
    {
        sprintf_s(_stdExceptionWhatBuf, "0x%X", hr);
        swprintf_s(_whatBuf, L"0x%X", hr);
    }

    HrError(const HRESULT hr, const wchar_t * what, const wchar_t * where)
        : Error(what, where), _hr(hr)
    { }

    HRESULT GetHR() const {
        return _hr;
    }

    const wchar_t * GetHrDescription() const;

private:
    HRESULT _hr;
    mutable wchar_t _hrDescriptionBuf[128] = {};
};

#define HR_ERROR(hr) throw adbook::HrError(hr, __FUNCTIONW__)

#define INVALID_PARAM_TYPE  "Invalid parameter's type"

#define MY_TRACE_HRERROR(e) MY_TRACE(L"HrError: hr:%d what:%s where:%s", e.GetHR(), e.What(), e.Where());

class Sqlite3Error : public Error
{
public:
    Sqlite3Error(const int err, const wchar_t * what, const wchar_t * where) :
        Error(what, where), errCode_(err)
    { }

    int GetErrCode() const {
        return errCode_;
    }

private:
    int errCode_{};
};

}   // namespace adbook


