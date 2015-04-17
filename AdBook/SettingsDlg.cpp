// SettingsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AdBook.h"
#include "error.h"
#include "SettingsDlg.h"
#include "afxdialogex.h"
#include "AdConnector.h"

// CSettingsDlg dialog

IMPLEMENT_DYNAMIC(CSettingsDlg, CDialogEx)

CSettingsDlg::CSettingsDlg(AppSettings & appSettings, CWnd* pParent /*=NULL*/)
	: CDialogEx(CSettingsDlg::IDD, pParent), appSettings_(appSettings)
{

}

CSettingsDlg::~CSettingsDlg()
{
}

const AppSettings & CSettingsDlg::GetAppSettings() const
{
    return appSettings_;
}

BOOL CSettingsDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    
    defaultPasswordChar_ = password_.GetPasswordChar();
    const UINT reasonableMaxPassLen = 40;
    password_.SetLimitText(reasonableMaxPassLen);
    
    CheckDlgButton(IDC_CHECK_CLOSE_TO_TRAY, appSettings_.GetMainWndSettings().CloseToTray() ? BST_CHECKED: BST_UNCHECKED);
    CheckDlgButton(IDC_CHECK_MINIMIZE_TO_TRAY, appSettings_.GetMainWndSettings().MinimizeToTray() ? BST_CHECKED : BST_UNCHECKED);
    const ConnectionSettings & cs = appSettings_.GetConnectionSettings();
    SetDlgItemText(IDC_EDIT_DC, cs.GetDC());
    SetDlgItemText(IDC_EDIT_USERNAME, cs.GetLogin());
    SetDlgItemText(IDC_EDIT_PASSWORD, cs.GetPassword());
    CheckDlgButton(IDC_CHECK_USER_DOMAIN, appSettings_.GetConnectionSettings().CurrentDomain() ? BST_CHECKED : BST_UNCHECKED);
    CheckDlgButton(IDC_CHECK_USE_CURUSERCRED, appSettings_.GetConnectionSettings().CurrentUserCredentials() ? BST_CHECKED : BST_UNCHECKED);
    CheckDlgButton(IDC_CHECK_FORGET_PASS, appSettings_.GetConnectionSettings().ForgetPassword() ? BST_CHECKED : BST_UNCHECKED);
    CheckDlgButton(IDC_CHECK_DISPLAY_PASS, appSettings_.GetConnectionSettings().DisplayPassword() ? BST_CHECKED : BST_UNCHECKED);    
    OnBnClickedCheckUseCurusercred();
    OnBnClickedCheckUserDomain();
    OnBnClickedCheckDisplayPassword();
    return TRUE;
}

void CSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT_PASSWORD, password_);
}


BEGIN_MESSAGE_MAP(CSettingsDlg, CDialogEx)
    ON_BN_CLICKED(IDOK, &CSettingsDlg::OnBnClickedOk)    
    ON_BN_CLICKED(IDC_CHECK_USE_CURUSERCRED, &CSettingsDlg::OnBnClickedCheckUseCurusercred)
    ON_BN_CLICKED(IDC_CHECK_USER_DOMAIN, &CSettingsDlg::OnBnClickedCheckUserDomain)
    ON_BN_CLICKED(IDC_CHECK_DISPLAY_PASSWORD, &CSettingsDlg::OnBnClickedCheckDisplayPassword)
    ON_BN_CLICKED(IDC_BUTTON_VERIFY, &CSettingsDlg::OnBnClickedButtonVerify)
END_MESSAGE_MAP()

// CSettingsDlg message handlers

void CSettingsDlg::ProcessUserInput()
{
    appSettings_.GetMainWndSettings().CloseToTray(IsDlgButtonChecked(IDC_CHECK_CLOSE_TO_TRAY) == BST_CHECKED);
    appSettings_.GetMainWndSettings().MinimizeToTray(IsDlgButtonChecked(IDC_CHECK_MINIMIZE_TO_TRAY) == BST_CHECKED);

    CString dc;
    ConnectionSettings & cs = appSettings_.GetConnectionSettings();
    cs.CurrentDomain(IsDlgButtonChecked(IDC_CHECK_USER_DOMAIN) == BST_CHECKED);
    GetDlgItemText(IDC_EDIT_DC, dc);
    dc.Trim();
    if (!cs.CurrentDomain() && dc.IsEmpty())
    {
        AfxMessageBox(IDS_ENTER_DC_NAME, MB_ICONEXCLAMATION);
        GetDlgItem(IDC_EDIT_DC)->SetFocus();
        throw HrError(E_INVALIDARG);
    }
    cs.SetDC(dc);

    cs.CurrentUserCredentials(IsDlgButtonChecked(IDC_CHECK_USE_CURUSERCRED) == BST_CHECKED);
    CString login;
    GetDlgItemText(IDC_EDIT_USERNAME, login);
    login.Trim();
    cs.SetLogin(login);

    CString password;
    GetDlgItemText(IDC_EDIT_PASSWORD, password);
    password.Trim();
    cs.SetPassword(password);

    if (!cs.CurrentUserCredentials())
    {
        if (login.IsEmpty())
        {
            AfxMessageBox(IDS_ENTER_LOGIN, MB_ICONEXCLAMATION);
            GetDlgItem(IDC_EDIT_USERNAME)->SetFocus();
            throw HrError(E_INVALIDARG);
        }
        if (password.IsEmpty())
        {
            AfxMessageBox(IDS_ENTER_PASSWORD, MB_ICONEXCLAMATION);
            GetDlgItem(IDC_EDIT_PASSWORD)->SetFocus();
            throw HrError(E_INVALIDARG);
        }
    }
    cs.DisplayPassword(IsDlgButtonChecked(IDC_CHECK_DISPLAY_PASS) == BST_CHECKED);
    cs.ForgetPassword(IsDlgButtonChecked(IDC_CHECK_FORGET_PASS) == BST_CHECKED);
}

void CSettingsDlg::OnBnClickedOk()
{
    try
    {
        ProcessUserInput();
        CDialogEx::OnOK();
    }
    catch (const HrError & )
    {
        
    }    
}

void CSettingsDlg::OnBnClickedCheckUseCurusercred()
{
    const bool useCurUserCred = (IsDlgButtonChecked(IDC_CHECK_USE_CURUSERCRED) == BST_CHECKED);
    GetDlgItem(IDC_EDIT_USERNAME)->EnableWindow(!useCurUserCred);
    GetDlgItem(IDC_EDIT_PASSWORD)->EnableWindow(!useCurUserCred);
    GetDlgItem(IDC_CHECK_DISPLAY_PASSWORD)->EnableWindow(!useCurUserCred);
    GetDlgItem(IDC_CHECK_FORGET_PASS)->EnableWindow(!useCurUserCred);
}

void CSettingsDlg::OnBnClickedCheckUserDomain()
{
    const bool useCurDomain = (IsDlgButtonChecked(IDC_CHECK_USER_DOMAIN) == BST_CHECKED);
    GetDlgItem(IDC_EDIT_DC)->EnableWindow(!useCurDomain);
}

void CSettingsDlg::OnBnClickedCheckDisplayPassword()
{
    const bool display = (IsDlgButtonChecked(IDC_CHECK_DISPLAY_PASSWORD) == BST_CHECKED);
    password_.SetPasswordChar(display ? 0 : defaultPasswordChar_);
    password_.Invalidate();
}

void CSettingsDlg::OnBnClickedButtonVerify()
{
    try
    {
        ProcessUserInput();
    }
    catch (const std::exception&)
    {
        return;
    }
        
    try
    {
        AdConnector ac(appSettings_.GetConnectionSettings());
        CWaitCursor wc;
        ac.Connect();
        AfxMessageBox(IDS_CONNECTION_SUCCEEDED);        
    }
    catch (const Error & e)
    {
        CString error;
        VERIFY(error.LoadString(IDS_FAILED_TO_CONNECT));
        error += L" ";
        error += e.What();
        AfxMessageBox(error, MB_ICONERROR);
    }
    catch (const std::exception & )
    {
        AfxMessageBox(IDS_FAILED_TO_CONNECT, MB_ICONERROR);
    }    
}
