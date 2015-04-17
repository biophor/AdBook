#pragma once

#include "AppSettings.h"
#include "afxwin.h"
// CSettingsDlg dialog

class CSettingsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSettingsDlg)

public:
	CSettingsDlg(AppSettings & appSettings, CWnd* pParent = NULL);   // standard constructor
	virtual ~CSettingsDlg();

    const AppSettings & GetAppSettings() const;

    BOOL OnInitDialog();
// Dialog Data
	enum { IDD = IDD_SETTINGS};

private:
    AppSettings appSettings_;
    CEdit password_;
    wchar_t defaultPasswordChar_;
private:
    void ProcessUserInput();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOk();    
    afx_msg void OnBnClickedCheckUseCurusercred();
    afx_msg void OnBnClickedCheckUserDomain();
    afx_msg void OnBnClickedCheckDisplayPassword();
    
    afx_msg void OnBnClickedButtonVerify();
};
