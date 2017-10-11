// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/*
Copyright (C) 2015-2020 Goncharov Andrei.

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

// SettingsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AdBook.h"
#include "SettingsDlg.h"
#include "afxdialogex.h"

// CSettingsDlg dialog

IMPLEMENT_DYNAMIC(CSettingsDlg, CDialogEx)

CSettingsDlg::CSettingsDlg(
    std::shared_ptr<adbook::AbstractAdAccessFactory> adAccessFactory,
    const adbook::ConnectionParams & connectionParams,
    CWnd* pParent /*=NULL*/
)
    : CDialogEx(CSettingsDlg::IDD, pParent),
    _adAccessFactory{ adAccessFactory },
    _connectionParams{ connectionParams }
{

}

CSettingsDlg::~CSettingsDlg() = default;

adbook::ConnectionParams CSettingsDlg::GetConnectionParams() const
{
    return _connectionParams;
}

BOOL CSettingsDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    _defaultPasswordChar = _password.GetPasswordChar();
    const UINT reasonableMaxPassLen = 40;
    _password.SetLimitText(reasonableMaxPassLen);

    SetDlgItemText(IDC_EDIT_DC, _connectionParams.GetDomainController().c_str());
    SetDlgItemText(IDC_EDIT_USERNAME, _connectionParams.GetLogin().c_str());
    SetDlgItemText(IDC_EDIT_PASSWORD, _connectionParams.GetPassword().c_str());
    CheckDlgButton(IDC_CHECK_USER_DOMAIN, _connectionParams.CurrentDomain() ? BST_CHECKED : BST_UNCHECKED);
    CheckDlgButton(IDC_CHECK_USE_CURUSERCRED, _connectionParams.IsCurrentUserCredentialsUsed() ? BST_CHECKED : BST_UNCHECKED);
    OnBnClickedCheckUseCurusercred();
    OnBnClickedCheckUserDomain();
    OnBnClickedCheckDisplayPassword();
    return TRUE;
}

void CSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT_PASSWORD, _password);
}

BEGIN_MESSAGE_MAP(CSettingsDlg, CDialogEx)
    ON_BN_CLICKED(IDOK, &CSettingsDlg::OnBnClickedOk)
    ON_BN_CLICKED(IDC_CHECK_USE_CURUSERCRED, &CSettingsDlg::OnBnClickedCheckUseCurusercred)
    ON_BN_CLICKED(IDC_CHECK_USER_DOMAIN, &CSettingsDlg::OnBnClickedCheckUserDomain)
    ON_BN_CLICKED(IDC_CHECK_DISPLAY_PASSWORD, &CSettingsDlg::OnBnClickedCheckDisplayPassword)
    ON_BN_CLICKED(IDC_BUTTON_VERIFY, &CSettingsDlg::OnBnClickedButtonVerify)
END_MESSAGE_MAP()

// CSettingsDlg message handlers

bool CSettingsDlg::ProcessUserInput()
{
    CString domainController;
    _connectionParams.ConnectDomainYouAreLoggedIn(IsDlgButtonChecked(IDC_CHECK_USER_DOMAIN) == BST_CHECKED);
    GetDlgItemText(IDC_EDIT_DC, domainController);
    domainController.Trim();
    if (!_connectionParams.CurrentDomain() && domainController.IsEmpty())
    {
        AfxMessageBox(IDS_ENTER_DC_NAME, MB_ICONEXCLAMATION);
        GetDlgItem(IDC_EDIT_DC)->SetFocus();
        return false;
    }
    _connectionParams.SetDomainController(domainController);

    _connectionParams.UseCurrentUserCredentials(IsDlgButtonChecked(IDC_CHECK_USE_CURUSERCRED) == BST_CHECKED);
    CString login;
    GetDlgItemText(IDC_EDIT_USERNAME, login);
    login.Trim();
    _connectionParams.SetLogin(login);
    CString password;
    GetDlgItemText(IDC_EDIT_PASSWORD, password);
    password.Trim();
    _connectionParams.SetPassword(password);

    if (!_connectionParams.IsCurrentUserCredentialsUsed())
    {
        if (login.IsEmpty())
        {
            AfxMessageBox(IDS_ENTER_LOGIN, MB_ICONEXCLAMATION);
            GetDlgItem(IDC_EDIT_USERNAME)->SetFocus();
            return false;
        }
        if (password.IsEmpty())
        {
            AfxMessageBox(IDS_ENTER_PASSWORD, MB_ICONEXCLAMATION);
            GetDlgItem(IDC_EDIT_PASSWORD)->SetFocus();
            return false;
        }
    }
    return true;
}

void CSettingsDlg::OnBnClickedOk()
{
    if (!ProcessUserInput()) {
        return;
    }
    CDialogEx::OnOK();
}

void CSettingsDlg::OnBnClickedCheckUseCurusercred()
{
    const bool useCurUserCred = (IsDlgButtonChecked(IDC_CHECK_USE_CURUSERCRED) == BST_CHECKED);
    GetDlgItem(IDC_EDIT_USERNAME)->EnableWindow(!useCurUserCred);
    GetDlgItem(IDC_EDIT_PASSWORD)->EnableWindow(!useCurUserCred);
    GetDlgItem(IDC_CHECK_DISPLAY_PASSWORD)->EnableWindow(!useCurUserCred);
}

void CSettingsDlg::OnBnClickedCheckUserDomain()
{
    const bool useCurDomain = (IsDlgButtonChecked(IDC_CHECK_USER_DOMAIN) == BST_CHECKED);
    GetDlgItem(IDC_EDIT_DC)->EnableWindow(!useCurDomain);
}

void CSettingsDlg::OnBnClickedCheckDisplayPassword()
{
    const bool display = (IsDlgButtonChecked(IDC_CHECK_DISPLAY_PASSWORD) == BST_CHECKED);
    _password.SetPasswordChar(display ? 0 : _defaultPasswordChar);
    _password.Invalidate();
}

void CSettingsDlg::OnBnClickedButtonVerify()
{
    if (!ProcessUserInput()) {
        return;
    }
    try
    {
        auto connector = _adAccessFactory->CreateConnector();
        CWaitCursor wc;
        connector->Connect(_connectionParams);
        AfxMessageBox(IDS_CONNECTION_SUCCEEDED, MB_ICONINFORMATION);
    }
    catch (const adbook::Error & e)
    {
        CString error;
        VERIFY(error.LoadString(IDS_FAILED_TO_CONNECT));
        error += L" ";
        error += e.What().c_str();
        AfxMessageBox(error, MB_ICONERROR);
    }
    catch (const std::exception & )
    {
        AfxMessageBox(IDS_FAILED_TO_CONNECT, MB_ICONERROR);
    }
}
