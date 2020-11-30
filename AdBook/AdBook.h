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

#pragma once

#ifndef __AFXWIN_H__
    #error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"        // main symbols
#include "AppSettings.h"


// CAdBookApp:
// See AdBook.cpp for the implementation of this class
//

class CAdBookApp : public CWinApp
{
public:
    CAdBookApp();
    ~CAdBookApp();
    const AppSettings & GetAppSettings() const;
    AppSettings & GetAppSettings();
// Overrides
public:
    virtual BOOL InitInstance();
private:
    void DisplayUI();
    void LoadSettings();
    void SaveSettings();
    void CreateAdAccessFactory();
private:
    std::shared_ptr<adbook::AbstractAdAccessFactory> _adAccessFactory;

    AppSettings _appSet;
    ULONG_PTR _gdiplusToken = 0;
    Gdiplus::GdiplusStartupInput _gdiplusStartupInput;
public:
// Implementation

    DECLARE_MESSAGE_MAP()
};

extern CAdBookApp theApp;
