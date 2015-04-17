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

// SvAttrEditor.cpp : implementation file
//

#include "stdafx.h"
#include "AdBook.h"
#include "SvAttrEditor.h"
#include "afxdialogex.h"

// SvAttrEditor dialog

IMPLEMENT_DYNAMIC(SvAttrEditor, CDialogEx)

SvAttrEditor::SvAttrEditor(adbook::AdPersonDesc & person, const adbook::Attributes::AttrId attrToChange, CWnd* pParent /*=NULL*/) //-V730
	: CDialogEx(SvAttrEditor::IDD, pParent), person_(person), attrToChange_(attrToChange)
    , newValue_(_T(""))
{
    icon_ = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

SvAttrEditor::~SvAttrEditor()
{
}

CString SvAttrEditor::GetNewValue() const
{
    return newValue_;
}

BOOL SvAttrEditor::OnInitDialog()
{
    CDialog::OnInitDialog();

    SetIcon(icon_, TRUE);
    SetIcon(icon_, FALSE);

    CRect rc;
    GetWindowRect(&rc);
    minSize_.cx = rc.right - rc.left;
    minSize_.cy = rc.bottom - rc.top;

    wndAnchor_.Init(GetSafeHwnd());
    wndAnchor_.Add(IDOK, WindowAnchor::BottomRight);
    wndAnchor_.Add(IDCANCEL, WindowAnchor::BottomRight);
    wndAnchor_.Add(IDC_EDIT_NAME, WindowAnchor::TopLeft | WindowAnchor::Right);
    wndAnchor_.Add(IDC_EDIT_CURVALUE, WindowAnchor::TopLeft | WindowAnchor::Right);
    wndAnchor_.Add(IDC_EDIT_NEWVALUE, WindowAnchor::TopLeft | WindowAnchor::Right);
    wndAnchor_.Add(IDC_EDIT_DN, WindowAnchor::TopLeft | WindowAnchor::Right);

    const CRect rect = theApp.GetAppSettings().GetSvChangeAttrDlgPos();
    if (!rect.IsRectEmpty())
    {
        MoveWindow(rect.left, rect.top, rect.Width(), rect.Height());
    }
    
    SetDlgItemText(IDC_EDIT_DN, person_.GetDn().c_str());
    CString curValue, attrName;
    auto & a = adbook::Attributes::GetInstance();
    SendDlgItemMessage(IDC_EDIT_NEWVALUE, EM_SETLIMITTEXT, a.GetAttrMaxLength(attrToChange_), 0);
    const CString uiAttrName = a.GetUiAttrName(attrToChange_).c_str();
    const CString ldapAttrName = a.GetLdapAttrName(attrToChange_).c_str();
    const CString attrOid = a.GetAttrOid(attrToChange_).c_str();
    if (person_.IsAttributeSet(static_cast<PCWSTR>(ldapAttrName)))
    {        
        curValue = person_.GetStringAttr(static_cast<PCWSTR>(ldapAttrName)).c_str();
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
    DDX_Text(pDX, IDC_EDIT_NEWVALUE, newValue_);
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
    wndAnchor_.OnSize();    
}


void SvAttrEditor::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
    if (minSize_.cx && minSize_.cy)
    {
        lpMMI->ptMinTrackSize.x = minSize_.cx;
        lpMMI->ptMinTrackSize.y = minSize_.cy;
        lpMMI->ptMaxTrackSize.y = minSize_.cy;
    }    
    CDialogEx::OnGetMinMaxInfo(lpMMI);
}


void SvAttrEditor::OnBnClickedOk()
{
    UpdateData();
    newValue_.Trim();
    CDialogEx::OnOK();
}

void SvAttrEditor::OnDestroy()
{
    CRect rect;
    GetWindowRect(&rect);
    theApp.GetAppSettings().SetChangeSvAttrDlgPos(rect);
    CDialogEx::OnDestroy();
}
