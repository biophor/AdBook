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

#pragma once

#include "AdBookBLExport.h"

namespace adbook
{

class ADBOOKBL_API Error : public std::exception
{
public:
    Error(const wchar_t * where = nullptr)
    {
        if (where)
        {
            wcscpy_s(whereBuf_, where);
        }
    }

    virtual std::wstring What() const = 0;
    std::wstring Where() const
    {
        return whereBuf_;
    }

    virtual const char * what() const
    {
        return whatBuf_;
    }

protected:
    mutable wchar_t errorBuf_[512] = {};
    char whatBuf_[64] = {};
    wchar_t whereBuf_[128] = {};
};


class ADBOOKBL_API HrError : public Error
{
public:
    HrError(const HRESULT hr, const wchar_t * where = nullptr) : Error(where), hr_(hr)
    {
        sprintf_s(whatBuf_, "0x%X.", hr);
    }

    virtual std::wstring What() const;

private:
    HRESULT hr_;
};

#define HR_ERROR(hr) throw adbook::HrError(hr,__FUNCTIONW__)

#define INVALID_PARAM_TYPE  "Invalid parameter's type"


}   // namespace adbook


