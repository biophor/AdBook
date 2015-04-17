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

class TextClipboard
{
public:
    TextClipboard();
    ~TextClipboard();

    void Put(const CString & text);

    TextClipboard(const TextClipboard &) = delete;
    TextClipboard(TextClipboard &&) = delete;
    void operator = (const TextClipboard &) = delete;
    void operator = (TextClipboard &&) = delete;    
private:
    void Alloc(const size_t sizeInBytes);
    void Lock();
    void Unlock() noexcept;
    bool IsLocked() const noexcept;

private:
    HGLOBAL clipboardDataHandle_ = nullptr;
    wchar_t * data_ = nullptr;
};

