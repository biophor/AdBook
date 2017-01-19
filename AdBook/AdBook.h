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
    AppSettings appSet_;
    ULONG_PTR gdiplusToken_ = 0;
    Gdiplus::GdiplusStartupInput gdiplusStartupInput_;
public:
// Implementation

    DECLARE_MESSAGE_MAP()
};

extern CAdBookApp theApp;
