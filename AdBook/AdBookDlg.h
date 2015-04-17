
// AdBookDlg.h : header file
//

#pragma once
#include "WindowAnchor.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "AdSearcher.h"
#include "LdapRequest.h"
#include "ContactFinder.h"
#include "MainWndSettings.h"

// CAdBookDlg dialog
class CAdBookDlg : public CDialogEx
{
// Construction
public:
    CAdBookDlg(CWnd * pParent = NULL);

// Dialog Data
    enum { IDD = IDD_ADBOOK_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange * pDX);	// DDX/DDV support

private:
    enum SearchFilterColIds { AttrColumnId = 0, ConditionColumnId = 1, ValueColumnId = 2 };
    enum CompositeAttrId { AllAttributes = LdapRequest::AttrIdEnd };

    void InitAttrList();
    void InitConditionList();
    void InitSearchFiltersList();
    void InitSearchResultList();
    void InitMinSize();
    
    void SaveWindowPosition();
    void RestoreWindowPosition();
    void SaveSearchFilterCols();
    
    void UpdateAddButtonState();
    void UpdateRemoveButtonState();
    LdapRequest ConstructLdapRequest();
    void OnNewItem(std::unique_ptr<AdSearcher::Item> itemPtr);
    void OnStart();
    void OnStop();
    LRESULT OnUmRefreshTotal(WPARAM, LPARAM);    
    LRESULT OnUmStart(WPARAM, LPARAM);
    LRESULT OnUmStop(WPARAM, LPARAM);


    AdSearcher adSearcher_ = theApp.GetAppSettings().GetConnectionSettings();
    MainWndSettings & mws_ = theApp.GetAppSettings().GetMainWndSettings();
    WindowAnchor wndAnchor_;
    CSize minSize_;
    ContactFinder contactFinder_;
    CComboBox attrList_;
    CComboBox condList_;
    CComboBox valList_;
    CListCtrl searchFilters_;
    CListCtrl searchResults_;
    std::recursive_mutex searchResultsMutex_;
    std::map<int, LdapRequest::AttrId> searchResultsColIdx_;    // <colId, attrId>
    std::deque<AdSearcher::Item> contacts_;

// Implementation
protected:
	HICON m_hIcon;
    
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
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
    afx_msg void OnLvnItemActivateListFilters(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnLvnItemchangedListFilters(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnLvnGetdispinfoListResults(NMHDR *pNMHDR, LRESULT *pResult);
};
