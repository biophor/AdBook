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

// SvAttrEditor.cpp : implementation file
//

#include "stdafx.h"
#include "AdBook.h"
#include "SvAttrEditor.h"
#include "afxdialogex.h"

// SvAttrEditor dialog

IMPLEMENT_DYNAMIC(SvAttrEditor, CDialogEx)

SvAttrEditor::SvAttrEditor(
    adbook::AdPersonDesc & person,
    const adbook::Attributes::AttrId attrToChange, CWnd* pParent /*=NULL*/
) //-V730
    : CDialogEx(SvAttrEditor::IDD, pParent)
    , _person{ person }
    , _attrToChange{ attrToChange }
    , _newValue{ _T("") }
{
    _icon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

SvAttrEditor::~SvAttrEditor()
{
}

CString SvAttrEditor::GetNewValue() const
{
    return _newValue;
}

BOOL SvAttrEditor::OnInitDialog()
{
    CDialog::OnInitDialog();

    SetIcon(_icon, TRUE);
    SetIcon(_icon, FALSE);

    CRect rc;
    GetWindowRect(&rc);
    _minSize.cx = rc.right - rc.left;
    _minSize.cy = rc.bottom - rc.top;

    _wndAnchor.Init(GetSafeHwnd());
    _wndAnchor.Add(IDOK, WindowAnchor::BottomRight);
    _wndAnchor.Add(IDCANCEL, WindowAnchor::BottomRight);
    _wndAnchor.Add(IDC_EDIT_NAME, WindowAnchor::TopLeft | WindowAnchor::Right);
    _wndAnchor.Add(IDC_EDIT_CURVALUE, WindowAnchor::TopLeft | WindowAnchor::Right);
    _wndAnchor.Add(IDC_EDIT_NEWVALUE, WindowAnchor::TopLeft | WindowAnchor::Right);
    _wndAnchor.Add(IDC_EDIT_DN, WindowAnchor::TopLeft | WindowAnchor::Right);

    const CRect rect = theApp.GetAppSettings().GetSvAttrEditorPosition();
    if (!rect.IsRectEmpty())
    {
        MoveWindow(rect.left, rect.top, rect.Width(), rect.Height());
    }

    SetDlgItemText(IDC_EDIT_DN, _person.GetDn().c_str());
    CString curValue, attrName;

    auto & attributes = adbook::Attributes::GetInstance();
    SendDlgItemMessage(IDC_EDIT_NEWVALUE, EM_SETLIMITTEXT, attributes.GetAttrMaxLength(_attrToChange), 0);
    const CString uiAttrName = attributes.GetUiAttrName(_attrToChange).c_str();
    const CString ldapAttrName = attributes.GetLdapAttrName(_attrToChange).c_str();
    const CString attrOid = attributes.GetAttrOid(_attrToChange).c_str();
    if (_person.IsAttributeSet(static_cast<PCWSTR>(ldapAttrName)))
    {
        curValue = _person.GetStringAttr(static_cast<PCWSTR>(ldapAttrName)).c_str();
        SetDlgItemText(IDC_EDIT_CURVALUE, curValue);
        SetDlgItemText(IDC_EDIT_NEWVALUE, curValue);
    }
    attrName = uiAttrName + L" / " + ldapAttrName + L" / " + attrOid;
    SetDlgItemText(IDC_EDIT_NAME, attrName);
    GetDlgItem(IDC_EDIT_NEWVALUE)->SetFocus();
    return FALSE;
}

void SvAttrEditor::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_NEWVALUE, _newValue);
}

BEGIN_MESSAGE_MAP(SvAttrEditor, CDialogEx)
    ON_WM_SIZE()
    ON_WM_GETMINMAXINFO()
    ON_BN_CLICKED(IDOK, &SvAttrEditor::OnBnClickedOk)
    ON_WM_DESTROY()
END_MESSAGE_MAP()

// SvAttrEditor message handlers

void SvAttrEditor::OnSize(UINT nType, int cx, int cy)
{
    CDialogEx::OnSize(nType, cx, cy);
    _wndAnchor.OnSize();
}

void SvAttrEditor::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
    if (_minSize.cx && _minSize.cy)
    {
        lpMMI->ptMinTrackSize.x = _minSize.cx;
        lpMMI->ptMinTrackSize.y = _minSize.cy;
        lpMMI->ptMaxTrackSize.y = _minSize.cy;
    }
    CDialogEx::OnGetMinMaxInfo(lpMMI);
}

void SvAttrEditor::OnBnClickedOk()
{
    UpdateData();
    _newValue.Trim();
    CDialogEx::OnOK();
}

void SvAttrEditor::OnDestroy()
{
    CRect rect;
    GetWindowRect(&rect);
    theApp.GetAppSettings().SetSvAttrEditorPosition(rect);
    CDialogEx::OnDestroy();
}


