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


// AdBook.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "AdBook.h"
#include "AdBookDlg.h"
#include "AppSettingsRegistryKeeper.h"

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

    // Place all significant initialization in InitInstance
    VERIFY(Gdiplus::Ok == Gdiplus::GdiplusStartup(&_gdiplusToken, &_gdiplusStartupInput, NULL));
}

CAdBookApp::~CAdBookApp()
{
    Gdiplus::GdiplusShutdown(_gdiplusToken);
}

// The one and only CAdBookApp object

CAdBookApp theApp;

// CAdBookApp initialization

void CAdBookApp::LoadSettings()
{
    try {
        AppSettingsRegistryKeeper appSetKeeper;
        bool failedToDecryptPassword = false;
        appSetKeeper.Load(_appSet, failedToDecryptPassword);

        if (failedToDecryptPassword) {
            // the main reason: the password was encrypted using different key
            AfxMessageBox(IDS_FAILED_TO_DECRYPT_PASSWORD, MB_ICONERROR);
        }
    }
    catch (const std::exception & e) {
        USES_CONVERSION;
        CString details = CA2WEX<256>(e.what());
        CString error;
        VERIFY(error.LoadString(IDS_FAILED_TO_LOAD_SETTINGS));
        error += L" " + details;
        AfxMessageBox(error, MB_ICONERROR);
    }
}

void CAdBookApp::SaveSettings()
{
    AppSettingsRegistryKeeper appSetKeeper;
    appSetKeeper.Save(_appSet);
}

void CAdBookApp::DisplayUI()
{
    CAdBookDlg dlg{ _adAccessFactory };
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

}

void CAdBookApp::CreateAdAccessFactory()
{
    _adAccessFactory = adbook::GetAdAccessFactory();
}

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

    SetThreadUILanguage(MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
    // Create the shell manager, in case the dialog contains
    // any shell tree view or shell list view controls.

    std::unique_ptr<CShellManager> pShellManager = std::make_unique<CShellManager>();

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

    LoadSettings();
    CreateAdAccessFactory();
    DisplayUI();
    SaveSettings();

    // Since the dialog has been closed, return FALSE so that we exit the
    // application, rather than start the application's message pump.

    // Delete the shell manager created above.

    pShellManager.reset();
    _adAccessFactory.reset();
    // cleanup static data to reduce the number of 'memory leak warnings'
    adbook::Attributes::GetInstance().PreExitUnload();

    return FALSE;
}


const AppSettings & CAdBookApp::GetAppSettings() const
{
    return _appSet;
}

AppSettings & CAdBookApp::GetAppSettings()
{
    return _appSet;
}

