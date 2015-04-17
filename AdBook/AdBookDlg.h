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

// AdBookDlg.h : header file
//

#pragma once
#include "WindowAnchor.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "ImageDisplayer.h"
#include "MainWndSettings.h"
#include "afxpropertygridctrl.h"

// CAdBookDlg dialog
class CAdBookDlg : public CDialogEx
{
// Construction
public:
    CAdBookDlg(CWnd * pParent = NULL);

// Dialog Data
    enum { IDD = IDD_ADBOOK_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange * pDX);    // DDX/DDV support

private:
    enum SearchFilterColId { AttrColumnId = 0, ConditionColumnId = 1, ValueColumnId = 2 };
    enum { AllAttributes = adbook::Attributes::AttrIdEnd + 1 };
    enum ContactDetailsColId { ContactDetailsAttrNameCol = 0, ContactDetailsAttrValueCol = 1 };

    void InitAnchor();
    void InitAttrList();
    void InitConditionList();
    void InitSearchFiltersList();
    void InitSearchResultList();
    void InitMinSize();
    void InitContactDetails();
        
    void SaveWindowPosition();
    void RestoreWindowPosition();
    void SaveSearchResults();
    void SaveSearchFilter();
    void SaveSearchFilterCols();
    void SaveSearchFilterStrings();
    void LoadSearchFilterStrings();
    void SaveSearchResultCols();    

    void CleanSortArrow();
    void Sort(const int colId, const bool ascending);
    void UpdateAddButtonState();
    void UpdateRemoveButtonState();
    void DisplayPersonDetails(const int personIdx);
    void RefreshPersonButtons(int personIdx = LB_ERR, int attrIdxLeft = LB_ERR, int attrIdxRight = LB_ERR);
    void ResizeContactDetailsCols();
    void RememberSuccessfulSearchString();
    void DisplaySvAttrEditor(CListCtrl & personDetails);
    int GetSelectedPersonIdx();
    void ApplyNewValue(adbook::AdPersonDesc & person, const adbook::Attributes::AttrId, const CString & newValue);
    adbook::LdapRequest ConstructLdapRequest();
    void OnNewItem(adbook::AdPersonDesc && item);
    void OnStart();
    void OnStop();
    LRESULT OnUmRefreshTotal(WPARAM, LPARAM);
    LRESULT OnUmStart(WPARAM, LPARAM);
    LRESULT OnUmStop(WPARAM, LPARAM);

    adbook::AdSearcher adSearcher_{ theApp.GetAppSettings().GetConnectionSettings() };
    MainWndSettings & mws_{ theApp.GetAppSettings().GetMainWndSettings() };
    WindowAnchor wndAnchor_;
    CSize minSize_;        
    std::map<int, adbook::Attributes::AttrId> searchResultsColIdx_;    // <colId, attrId>
    std::recursive_mutex personsMutex_;
    adbook::AdPersonDescList persons_;

    CComboBox attrList_;
    CComboBox condList_;
    CComboBox valList_;
    const size_t valListMaxItemCount = 32;
    CListCtrl searchFilters_;
    CListCtrl searchResults_;
    CListCtrl contactDetails1_;
    CListCtrl contactDetails2_;    
    ImageDisplayer photo_;

// Implementation
protected:
    HICON m_hIcon;
    
    // Generated message map functions
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg void OnSelectPhoto();
    afx_msg void OnClearPhoto();
    afx_msg HCURSOR OnQueryDragIcon();
    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnSize(UINT nType, int cx, int cy);        
    afx_msg void OnBnClickedButtonAdd();
    afx_msg void OnBnClickedButtonRemove();
    afx_msg void OnBnClickedButtonFind();
    afx_msg void OnBnClickedButtonSettings();
    afx_msg void OnBnClickedCancel();
    afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
    afx_msg void OnClose();
    afx_msg void OnCbnEditchangeComboText();
    afx_msg void OnCbnSelchangeComboText();    
    afx_msg void OnLvnItemchangedListFilters(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnLvnGetdispinfoListResults(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnHdnItemclickListResults(NMHDR *pNMHDR, LRESULT *pResult);       
    afx_msg void OnLvnItemchangedListResults(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnLvnItemchangedContactDetails(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnBnClickedButtonCopy1();
    afx_msg void OnBnClickedButtonChange1();
    afx_msg void OnBnClickedButtonCopy2();
    afx_msg void OnBnClickedButtonChange2();
    afx_msg void OnBnClickedButtonSelect();        
    afx_msg void OnNMDblclkContactDetails1(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnNMDblclkContactDetails2(NMHDR *pNMHDR, LRESULT *pResult);
};
