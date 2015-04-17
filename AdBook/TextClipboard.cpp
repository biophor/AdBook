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
#include "TextClipboard.h"


TextClipboard::TextClipboard()
{
    if (!OpenClipboard(NULL))
    {
        HR_ERROR(HRESULT_FROM_WIN32(GetLastError()));
    }
}

TextClipboard::~TextClipboard()
{
    VERIFY(CloseClipboard());
    if (IsLocked())
    {
        Unlock();
        clipboardDataHandle_ = nullptr;
    }
}

void TextClipboard::Alloc(const size_t sizeInBytes)
{
    if (clipboardDataHandle_ || !sizeInBytes)
    {
        HR_ERROR(E_UNEXPECTED);
    }
    clipboardDataHandle_ = GlobalAlloc(GMEM_MOVEABLE, sizeInBytes);
}

void TextClipboard::Lock()
{
    if (!clipboardDataHandle_)
    {
        HR_ERROR(E_UNEXPECTED);
    }
    data_ = static_cast<wchar_t *>(GlobalLock(clipboardDataHandle_));
}

void TextClipboard::Unlock() noexcept
{
    if (clipboardDataHandle_)
    {
        GlobalUnlock(clipboardDataHandle_);
        data_ = nullptr;
    }
}

bool TextClipboard::IsLocked() const noexcept
{
    return data_ != nullptr;
}

void TextClipboard::Put(const CString & text)
{
    if (text.IsEmpty())
    {
        HR_ERROR(E_INVALIDARG);
    }
    const size_t sizeInBytes = (text.GetLength() + 1) * sizeof(text.GetAt(0)); //-V568
    Alloc(sizeInBytes);
    Lock();
    StringCchCopy(data_, sizeInBytes / sizeof(wchar_t), text);
    Unlock();
    if (!SetClipboardData(CF_UNICODETEXT, clipboardDataHandle_))
    {        
        HR_ERROR(HRESULT_FROM_WIN32(GetLastError()));
    }
}

