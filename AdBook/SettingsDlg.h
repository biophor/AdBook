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

#include "AppSettings.h"
#include "afxwin.h"
// CSettingsDlg dialog

class CSettingsDlg : public CDialogEx
{
    DECLARE_DYNAMIC(CSettingsDlg)

public:
    CSettingsDlg(
        std::shared_ptr<adbook::AbstractAdAccessFactory> adAccessFactory,
        const adbook::ConnectionParams & connectionParams,
        CWnd * pParent = nullptr
    );
    virtual ~CSettingsDlg();

    adbook::ConnectionParams GetConnectionParams() const;

    BOOL OnInitDialog();
// Dialog Data
    enum { IDD = IDD_SETTINGS};

private:
    adbook::ConnectionParams _connectionParams;
    CEdit _password;
    wchar_t _defaultPasswordChar = 0;
    std::shared_ptr<adbook::AbstractAdAccessFactory> _adAccessFactory;
private:
    bool ProcessUserInput();
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
