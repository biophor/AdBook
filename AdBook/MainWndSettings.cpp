// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/*
Copyright (C) 2015-2020 Andrei Goncharov.

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
#include "MainWndSettings.h"

namespace
{

const wchar_t * mainWndSection = L"MainWindow";
const wchar_t * leftParam = L"left";
const wchar_t * rightParam = L"right";
const wchar_t * topParam = L"top";
const wchar_t * bottomParam = L"bottom";
const wchar_t * searchFilterColWidthParam = L"SearchFilterColWidth";
const wchar_t * allConditionsShouldBeMetParam = L"allConditionsShouldBeMet";
const wchar_t * searchResultColWidthParam = L"SearchResultColWidth";
const wchar_t * searchResultColOrderParam = L"SearchResultColOrder";
const wchar_t * searchFilterStringsParam = L"SearchFilterStrings";

}

void MainWndSettings::SetRect(const CRect & rect) noexcept
{
    rect_ = rect;
}

const CRect & MainWndSettings::GetRect() const noexcept
{
    return rect_;
}

const MainWndSettings::IntVector & MainWndSettings::GetFilterColWidth() const noexcept
{
    return filterColWidth_;
}

const MainWndSettings::IntVector & MainWndSettings::GetResultColWidth() const noexcept
{
    return resultColWidth_;
}

const MainWndSettings::IntVector & MainWndSettings::GetResultColOrder() const noexcept
{
    return resultColOrder_;
}

const MainWndSettings::SearchFilterStrings & MainWndSettings::GetSearchFilterStrings() const
{
    return searchFilterStrings_;
}


