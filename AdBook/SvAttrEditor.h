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
#include "WindowAnchor.h"

// SvAttrEditor dialog

// Dialog for editing single-valued string attributes
class SvAttrEditor : public CDialogEx
{
    DECLARE_DYNAMIC(SvAttrEditor)

public:
    SvAttrEditor(
        adbook::AdPersonDesc & person,
        const adbook::Attributes::AttrId attrToChange,
        CWnd* pParent = NULL
    );   // standard constructor
    virtual ~SvAttrEditor();

// Dialog Data
    enum { IDD = IDD_CHANGE_SV_ATTR };

    CString GetNewValue() const;
private:
    WindowAnchor _wndAnchor;
    CSize _minSize;
    const adbook::AdPersonDesc & _person;
    const adbook::Attributes::AttrId _attrToChange;
    HICON _icon;
protected:
    virtual BOOL OnInitDialog();
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
    afx_msg void OnBnClickedOk();
private:
    CString _newValue;
public:
    afx_msg void OnDestroy();
};
