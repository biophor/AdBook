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


// AdBook.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "AdBook.h"
#include "AdBookDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAdBookApp

BEGIN_MESSAGE_MAP(CAdBookApp, CWinApp)
    ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CAdBookApp construction

CAdBookApp::CAdBookApp() //-V730
{
    // support Restart Manager
    m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

    
    // TODO: add construction code here,
    // Place all significant initialization in InitInstance
    VERIFY(Gdiplus::Ok == Gdiplus::GdiplusStartup(&gdiplusToken_, &gdiplusStartupInput_, NULL));
}

CAdBookApp::~CAdBookApp()
{
    Gdiplus::GdiplusShutdown(gdiplusToken_);
}

// The one and only CAdBookApp object

CAdBookApp theApp;

// CAdBookApp initialization

BOOL CAdBookApp::InitInstance()
{
    // InitCommonControlsEx() is required on Windows XP if an application
    // manifest specifies use of ComCtl32.dll version 6 or later to enable
    // visual styles.  Otherwise, any window creation will fail.
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    // Set this to include all the common control classes you want to use
    // in your application.
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();


    AfxEnableControlContainer();

    // Create the shell manager, in case the dialog contains
    // any shell tree view or shell list view controls.
    CShellManager *pShellManager = new CShellManager;

    // Activate "Windows Native" visual manager for enabling themes in MFC controls
    CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

    // Standard initialization
    // If you are not using these features and wish to reduce the size
    // of your final executable, you should remove from the following
    // the specific initialization routines you do not need
    // Change the registry key under which our settings are stored
    // TODO: You should modify this string to be something appropriate
    // such as the name of your company or organization
    SetRegistryKey(_T("Active Directory Contact Book"));
    
    appSet_.LoadSettings();
    
    CAdBookDlg dlg;
    m_pMainWnd = &dlg;
    INT_PTR nResponse = dlg.DoModal();
    if (nResponse == IDOK)
    {
        // TODO: Place code here to handle when the dialog is
        //  dismissed with OK
    }
    else if (nResponse == IDCANCEL)
    {
        // TODO: Place code here to handle when the dialog is
        //  dismissed with Cancel
    }
    else if (nResponse == -1)
    {
        TRACE(traceAppMsg, 0, "Warning: dialog creation failed, so application is terminating unexpectedly.\n");
        TRACE(traceAppMsg, 0, "Warning: if you are using MFC controls on the dialog, you cannot #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS.\n");
    }

    // Delete the shell manager created above.
    if (pShellManager != NULL)
    {
        delete pShellManager;
    }    
    appSet_.SaveSettings();
    adbook::Attributes::GetInstance().PreExitUnload(); // prevent false positive warnings about memory leaks
    // Since the dialog has been closed, return FALSE so that we exit the
    //  application, rather than start the application's message pump.
    
    return FALSE;
}

const AppSettings & CAdBookApp::GetAppSettings() const
{
    return appSet_;
}

AppSettings & CAdBookApp::GetAppSettings()
{
    return appSet_;
}

