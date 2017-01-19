// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
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

void MainWndSettings::Save()
{
    CWinApp * app = AfxGetApp();        
    //main windows settings
    VERIFY(app->WriteProfileInt(mainWndSection, leftParam, GetRect().left));
    VERIFY(app->WriteProfileInt(mainWndSection, topParam, GetRect().top));
    VERIFY(app->WriteProfileInt(mainWndSection, rightParam, GetRect().right));
    VERIFY(app->WriteProfileInt(mainWndSection, bottomParam, GetRect().bottom));
    VERIFY(app->WriteProfileInt(mainWndSection, allConditionsShouldBeMetParam,
                                GetCondCombineOp() == ConditionsCombineOperation::And));

    // vector {23,32,34,193} -> string "23 32 34 193"
    auto v2s = [](const auto & v) {
        CString s;
        std::wstringstream ss;
        std::for_each(v.begin(), v.end(), [&ss](int d) {ss << d << L" ";});
        s = ss.str().c_str();
        return s;
    };
    VERIFY(app->WriteProfileStringW(mainWndSection, searchFilterColWidthParam, v2s(GetFilterColWidth())));
    VERIFY(app->WriteProfileStringW(mainWndSection, searchResultColWidthParam, v2s(GetResultColWidth())));
    VERIFY(app->WriteProfileStringW(mainWndSection, searchResultColOrderParam, v2s(GetResultColOrder())));
    
    auto ls2s = [](const auto & ls) {
        CString s;
        std::wstringstream ss;
        std::for_each(ls.begin(), ls.end(), [&ss](const CString & s) {ss << static_cast<const wchar_t *>(s) << L"\n"; });
        s = ss.str().c_str();
        return s;
    };
    VERIFY(app->WriteProfileStringW(mainWndSection, searchFilterStringsParam, ls2s(GetSearchFilterStrings())));
    // search filters
    searchFilters_.Save();
}

void MainWndSettings::Load()
{
    CWinApp * app = AfxGetApp();
    VERIFY(app);
    auto getws = [app](const wchar_t* section, const wchar_t * param) -> std::wstring {
        return static_cast<const wchar_t *>(app->GetProfileString(section, param));
    };
    auto getint = [app](const wchar_t* section, const wchar_t * param, int def) {
        return (app->GetProfileInt(section, param, def));
    };
    
    // main windows settings
    SetRect(CRect(getint(mainWndSection, leftParam, 0), getint(mainWndSection, topParam, 0),
                  getint(mainWndSection, rightParam, 0), getint(mainWndSection, bottomParam, 0)));

    SetCondCombineOp(getint(mainWndSection, allConditionsShouldBeMetParam, 1) ?
                          ConditionsCombineOperation::And :
                          ConditionsCombineOperation::Or);

    // string "23 32 34 193" -> vector {23,32,34,193}
    auto s2v = [](const std::wstring & s) {
        IntVector v;
        std::wstringstream ss(s);
        std::istream_iterator<int, wchar_t> iter(ss);
        std::istream_iterator<int, wchar_t> end;
        std::for_each(iter, end, [&v](int d) { v.push_back(d); });
        return v;
    };
    SetFilterColWidth(s2v(getws(mainWndSection, searchFilterColWidthParam)));
    SetResultColWidth(s2v(getws(mainWndSection, searchResultColWidthParam)));
    SetResultColOrder(s2v(getws(mainWndSection, searchResultColOrderParam)));

    auto s2ls = [](const std::wstring & s) {
        SearchFilterStrings v;
        std::wstringstream ss(s);
        std::wstring w;
        while (std::getline(ss, w, L'\n'))
        {
            v.push_back(w.c_str());
        }
        return v;
    };
    SetSearchFilterStrings(s2ls(getws(mainWndSection, searchFilterStringsParam)));    
    // search filters
    searchFilters_.Load();
}
