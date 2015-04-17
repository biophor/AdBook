
// AdBook.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include "AppSettings.h"

// CAdBookApp:
// See AdBook.cpp for the implementation of this class
//

class CAdBookApp : public CWinApp
{
public:
	CAdBookApp();

    const AppSettings & GetAppSettings() const;
    AppSettings & GetAppSettings();

// Overrides
public:
	virtual BOOL InitInstance();
    
private:
    
    
private:    
    AppSettings appSet_;
public:
// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CAdBookApp theApp;