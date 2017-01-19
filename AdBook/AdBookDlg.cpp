// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
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


// AdBookDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AdBook.h"
#include "AdBookDlg.h"
#include "afxdialogex.h"
#include "SettingsDlg.h"
#include "SvAttrEditor.h"
#include "TextClipboard.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const DWORD UM_REFRESH_TOTAL = (WM_USER + 1);
const DWORD UM_START = (WM_USER + 2);
const DWORD UM_STOP = (WM_USER + 3);

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
    CAboutDlg();

// Dialog Data
    enum { IDD = IDD_ABOUTBOX };

    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
} 

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CAdBookDlg dialog

CAdBookDlg::CAdBookDlg(CWnd* pParent /*=NULL*/) //-V730
    : CDialogEx(CAdBookDlg::IDD, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAdBookDlg::DoDataExchange(CDataExchange * pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_ATTR, attrList_);
    DDX_Control(pDX, IDC_COMBO_CONDITION, condList_);
    DDX_Control(pDX, IDC_COMBO_TEXT, valList_);
    DDX_Control(pDX, IDC_LIST_FILTERS, searchFilters_);
    DDX_Control(pDX, IDC_LIST_RESULTS, searchResults_);
    DDX_Control(pDX, IDC_CONTACT_DETAILS1, contactDetails1_);
    DDX_Control(pDX, IDC_CONTACT_DETAILS2, contactDetails2_);
    DDX_Control(pDX, IDC_STATIC_PHOTO, photo_);
}

void CAdBookDlg::InitAttrList()
{    
    CString anyAttrUiName;
    VERIFY(anyAttrUiName.LoadStringW(IDS_ATTR_ANY));
    VERIFY(attrList_.SetItemData(attrList_.AddString(anyAttrUiName), AllAttributes) != CB_ERR);

    const auto & a = adbook::Attributes::GetInstance();
    const auto ids = adbook::Attributes::GetInstance().GetAttrIds();
    for (const auto i : ids)
    {
        if (a.IsEditableString(i))
        {
            const auto displayName = a.GetUiAttrName(i);
            VERIFY(attrList_.SetItemData(attrList_.AddString(displayName.c_str()), i) != CB_ERR);
        }
    }
    attrList_.SetCurSel(0);
}

void CAdBookDlg::InitConditionList()
{
    CString ruleName;
    VERIFY(ruleName.LoadString(IDS_MATCH_RULE_CONTAINS));
    VERIFY(condList_.SetItemData(condList_.AddString(ruleName), adbook::LdapRequest::Contains) != CB_ERR);
    VERIFY(ruleName.LoadString(IDS_MATCH_RULE_START_WITH));
    VERIFY(condList_.SetItemData(condList_.AddString(ruleName), adbook::LdapRequest::BeginWith) != CB_ERR);
    VERIFY(ruleName.LoadString(IDS_MATCH_RULE_EXACT_MATCH));
    VERIFY(condList_.SetItemData(condList_.AddString(ruleName), adbook::LdapRequest::ExactMatch) != CB_ERR);
    VERIFY(ruleName.LoadString(IDS_MATCH_RULE_END_WITH));
    VERIFY(condList_.SetItemData(condList_.AddString(ruleName), adbook::LdapRequest::EndWith) != CB_ERR);
    condList_.SetCurSel(0);
}

void CAdBookDlg::InitSearchResultList()
{
    searchResults_.SetExtendedStyle(searchResults_.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER|
        LVS_EX_HEADERDRAGDROP | LVS_EX_LABELTIP);
    
    const int numCols = attrList_.GetCount();
    for (int i = 0, colIdx=0; i < numCols; ++i)
    {
        CString title;
        attrList_.GetLBText(i, title);
        const auto id = static_cast<adbook::Attributes::AttrId>(attrList_.GetItemData(i));
        if (!adbook::Attributes::GetInstance().IsAttrSupported(id))
        {
            continue;
        }
        searchResults_.InsertColumn(colIdx, title, LVCFMT_LEFT, 100);
        searchResultsColIdx_[colIdx] = id;
        ++colIdx;
    }
    
    searchResults_.SetColumnWidth(searchResults_.GetHeaderCtrl()->GetItemCount()-1, LVSCW_AUTOSIZE_USEHEADER);
    
    auto rcw = theApp.GetAppSettings().GetMainWndSettings().GetResultColWidth();
    const int colCount = searchResults_.GetHeaderCtrl()->GetItemCount();
    if (static_cast<int>(rcw.size()) == colCount)
    {
        for (int i = 0; i < colCount; ++i)
        {
            if (rcw[i])
            {
                searchResults_.SetColumnWidth(i, rcw[i]);
            }
        }
    }
    auto rco = theApp.GetAppSettings().GetMainWndSettings().GetResultColOrder();
    if (!rco.empty() && static_cast<int>(rco.size()) == colCount)
    {
        VERIFY(searchResults_.SetColumnOrderArray(static_cast<int>(rco.size()), &rco[0]));
    }
    try
    {        
        std::lock_guard<decltype(personsMutex_)> lg(personsMutex_);
        persons_.Load();
        OnUmRefreshTotal(persons_.size(), 0);
    }
    catch (const std::exception & e)
    {
        USES_CONVERSION;
        CString details = CA2WEX<256>(e.what());
        CString error;
        VERIFY(error.LoadString(IDS_FAILED_TO_LOAD_SEARCH_RESULT));
        error += L" " + details;
        AfxMessageBox(error, MB_ICONERROR);
    }
}

void CAdBookDlg::InitMinSize()
{
    CRect rc;
    GetWindowRect(&rc);
    minSize_.cx = rc.right - rc.left;
    minSize_.cy = rc.bottom - rc.top;
}

void CAdBookDlg::InitContactDetails()
{
    contactDetails1_.SetExtendedStyle(contactDetails1_.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER |
        LVS_EX_LABELTIP);
    contactDetails1_.InsertColumn(ContactDetailsAttrNameCol, L"", LVCFMT_LEFT, LVSCW_AUTOSIZE);
    contactDetails1_.InsertColumn(ContactDetailsAttrValueCol, L"", LVCFMT_LEFT, LVSCW_AUTOSIZE);

    contactDetails2_.SetExtendedStyle(contactDetails2_.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER |
        LVS_EX_LABELTIP);
    contactDetails2_.InsertColumn(ContactDetailsAttrNameCol, L"", LVCFMT_LEFT, LVSCW_AUTOSIZE);
    contactDetails2_.InsertColumn(ContactDetailsAttrValueCol, L"", LVCFMT_LEFT, LVSCW_AUTOSIZE);

    const auto & a = adbook::Attributes::GetInstance();
    const auto attrIds = a.GetAttrIds();
    for (size_t i = 0, num = attrIds.size(); i < num; ++i)
    {
        if (!a.IsEditableString(attrIds[i]))
        {
            continue;
        }
        const auto attrDisplayName = a.GetUiAttrName(attrIds[i]);
        if (i % 2 == 0)
        {
            const int idx = contactDetails1_.InsertItem(contactDetails1_.GetItemCount(), attrDisplayName.c_str());
            contactDetails1_.SetItemData(idx, attrIds[i]);
        }
        else
        {
            const int idx = contactDetails2_.InsertItem(contactDetails2_.GetItemCount(), attrDisplayName.c_str());
            contactDetails2_.SetItemData(idx, attrIds[i]);
        }
    }
    contactDetails1_.SetColumnWidth(0, LVSCW_AUTOSIZE);
    contactDetails2_.SetColumnWidth(0, LVSCW_AUTOSIZE);
}

void CAdBookDlg::InitAnchor()
{
    wndAnchor_.Init(GetSafeHwnd());    
    wndAnchor_.Add(IDOK, WindowAnchor::BottomRight);
    wndAnchor_.Add(IDCANCEL, WindowAnchor::BottomRight);
    wndAnchor_.Add(IDC_BUTTON_ADD, WindowAnchor::TopRight);
    wndAnchor_.Add(IDC_BUTTON_FIND, WindowAnchor::TopRight);
    wndAnchor_.Add(IDC_BUTTON_REMOVE, WindowAnchor::TopRight);
    wndAnchor_.Add(IDC_BUTTON_SETTINGS, WindowAnchor::BottomLeft);
    wndAnchor_.Add(IDC_COMBO_TEXT, WindowAnchor::TopLeft | WindowAnchor::Right);
    wndAnchor_.Add(IDC_LIST_FILTERS, WindowAnchor::TopLeft | WindowAnchor::BottomRight, 1.0f, 1.0f, 1.0f, 0.2f);
    wndAnchor_.Add(IDC_CONTACT_DETAILS_CAPTION, WindowAnchor::BottomLeft, 1.0f, 1.0f, 1.0f, 0.8f);
    wndAnchor_.Add(IDC_CONTACT_DETAILS1, WindowAnchor::TopLeft | WindowAnchor::BottomRight, 1.0f, 0.4f, 0.2f, 1.0f);
    wndAnchor_.Add(IDC_CONTACT_DETAILS2, WindowAnchor::TopLeft | WindowAnchor::BottomRight, 0.6f, 0.8f, 0.2f, 1.0f);
    wndAnchor_.Add(IDC_BUTTON_COPY1, WindowAnchor::BottomRight, 1.0f, 0.4f, 1.0f, 1.0f);
    wndAnchor_.Add(IDC_BUTTON_CHANGE1, WindowAnchor::BottomRight, 1.0f, 0.4f, 1.0f, 1.0f);
    wndAnchor_.Add(IDC_BUTTON_COPY2, WindowAnchor::BottomRight, 1.0f, 0.8f, 1.0f, 1.0f);
    wndAnchor_.Add(IDC_BUTTON_CHANGE2, WindowAnchor::BottomRight, 1.0f, 0.8f, 1.0f, 1.0f);
    wndAnchor_.Add(IDC_BUTTON_SELECT, WindowAnchor::BottomRight, 1.0f, 1.0f, 1.0f, 1.0f);
    wndAnchor_.Add(IDC_STATIC_PHOTO, WindowAnchor::TopLeft | WindowAnchor::BottomRight, 0.2f, 1.0f, 0.2f, 1.0f);
    wndAnchor_.Add(IDC_PHOTO_CAPTION, WindowAnchor::Left | WindowAnchor::BottomRight, 0.2f, 1.0f, 1.0f, 0.8f);
    wndAnchor_.Add(IDC_LIST_RESULTS, WindowAnchor::TopLeft | WindowAnchor::BottomRight, 1.0f, 1.0f, 0.8f, 0.8f);
    wndAnchor_.Add(IDC_RADIO_ONE, WindowAnchor::TopLeft | WindowAnchor::Bottom, 1.0f, 1.0f, 0.8f, 0.2f);
    wndAnchor_.Add(IDC_RADIO_ALL, WindowAnchor::TopLeft | WindowAnchor::Bottom, 1.0f, 1.0f, 0.8f, 0.2f);
    wndAnchor_.Add(IDC_RESULT_CAPTION, WindowAnchor::TopLeft | WindowAnchor::BottomRight, 1.0f, 1.0f, 0.8f, 0.2f);
    wndAnchor_.Add(IDC_STATIC_TOTAL, WindowAnchor::Top | WindowAnchor::BottomRight, 1.0f, 1.0f, 0.8f, 0.2f);
}

void CAdBookDlg::InitSearchFiltersList()
{
    searchFilters_.SetExtendedStyle(searchFilters_.GetExtendedStyle() | LVS_EX_FULLROWSELECT);

    CString colTitle;
    VERIFY(colTitle.LoadStringW(IDS_COL_TITLE_ATTR));
    searchFilters_.InsertColumn(AttrColumnId, colTitle, LVCFMT_LEFT, 100);

    VERIFY(colTitle.LoadStringW(IDS_COL_TITLE_CONDITION));
    searchFilters_.InsertColumn(ConditionColumnId, colTitle, LVCFMT_LEFT, 100);
    
    VERIFY(colTitle.LoadStringW(IDS_COL_TITLE_VALUE));
    searchFilters_.InsertColumn(ValueColumnId, colTitle, LVCFMT_LEFT, 100);
    
    searchFilters_.SetColumnWidth(ValueColumnId, LVSCW_AUTOSIZE_USEHEADER);
    
    const auto fcw = theApp.GetAppSettings().GetMainWndSettings().GetFilterColWidth();
    const int colCount = searchFilters_.GetHeaderCtrl()->GetItemCount();
    if (static_cast<int>(fcw.size()) == colCount)
    {        
        for (int i = 0; i < colCount; ++i)
        {
            if (fcw[i])
            {
                searchFilters_.SetColumnWidth(i, fcw[i]);
            }
        }
    }
    
    std::map<DWORD_PTR, CString> matchingRuleNames;    
    for (int i = 0, max = condList_.GetCount(); i < max; ++i)
    {
        DWORD_PTR data = condList_.GetItemData(i);
        CString displayName;
        condList_.GetLBText(i, displayName);
        matchingRuleNames[data] = displayName;
    }
    
    auto & attributes = adbook::Attributes::GetInstance();
    for (const auto & i : mws_.GetSearchFilters())
    {
        int idx;        
        adbook::Attributes::AttrId attrId = boost::numeric_cast<adbook::Attributes::AttrId>(i.attrId);
        if (attributes.IsAttrSupported(attrId))
        {
            idx = searchFilters_.InsertItem(searchFilters_.GetItemCount(), attributes.GetUiAttrName(attrId).c_str());
        }
        else
        {
            CString attrAny;
            VERIFY(attrAny.LoadStringW(IDS_ATTR_ANY));
            idx = searchFilters_.InsertItem(searchFilters_.GetItemCount(), attrAny);
        }
        searchFilters_.SetItemText(idx, ConditionColumnId, matchingRuleNames.at(i.rule));
        searchFilters_.SetItemText(idx, ValueColumnId, i.attrValue);
        const DWORD itemData = MAKELONG(i.attrId, i.rule);
        searchFilters_.SetItemData(idx, itemData);
    }
}

void CAdBookDlg::SaveSearchFilterCols()
{
    MainWndSettings::IntVector fcw;
    const int colCount = searchFilters_.GetHeaderCtrl()->GetItemCount();
    for (int i = 0; i < colCount; ++i)
    {
        const int colWidth = searchFilters_.GetColumnWidth(i);
        fcw.push_back(colWidth);
    }
    theApp.GetAppSettings().GetMainWndSettings().SetFilterColWidth(std::move(fcw));
}

void CAdBookDlg::SaveSearchFilterStrings()
{
    MainWndSettings::SearchFilterStrings sfs;
    const int count = valList_.GetCount();
    for (int i = 0; i < count; ++i)
    {
        CString s;
        valList_.GetLBText(i, s);
        s.Trim();
        if (!s.IsEmpty())
        {
            sfs.push_back(s);
        }
    }
    theApp.GetAppSettings().GetMainWndSettings().SetSearchFilterStrings(std::move(sfs));
}

void CAdBookDlg::LoadSearchFilterStrings()
{
    const auto & sfs = theApp.GetAppSettings().GetMainWndSettings().GetSearchFilterStrings();
    for (const auto & i : sfs)
    {        
        valList_.InsertString(valList_.GetCount(), i);
    }
}

void CAdBookDlg::SaveSearchResultCols()
{
    MainWndSettings::IntVector rcw;
    const int colCount = searchResults_.GetHeaderCtrl()->GetItemCount();
    for (int i = 0; i < colCount; ++i)
    {
        const int colWidth = searchResults_.GetColumnWidth(i);
        rcw.push_back(colWidth);
    }
    theApp.GetAppSettings().GetMainWndSettings().SetResultColWidth(std::move(rcw));

    if (colCount)
    {
        MainWndSettings::IntVector rco(static_cast<size_t>(colCount));
        VERIFY(searchResults_.GetColumnOrderArray(&rco[0], static_cast<int>(rco.size())));
        theApp.GetAppSettings().GetMainWndSettings().SetResultColOrder(std::move(rco));
    }
}

void CAdBookDlg::CleanSortArrow()
{
    CHeaderCtrl * headerCtrl = searchResults_.GetHeaderCtrl();
    for (int i = 0, count = headerCtrl->GetItemCount(); i < count; ++i)
    {
        HDITEM hdItem{ HDI_FORMAT };
        VERIFY(headerCtrl->GetItem(i, &hdItem));
        hdItem.fmt &= ~HDF_SORTDOWN;
        hdItem.fmt &= ~HDF_SORTUP;
        VERIFY(headerCtrl->SetItem(i, &hdItem));
    }
}

void CAdBookDlg::SaveSearchResults()
{
    try
    {
        CWaitCursor wc;
        std::lock_guard<decltype(personsMutex_)> lg(personsMutex_);
        persons_.Save();
    }
    catch (const std::exception & e)
    {        
        USES_CONVERSION;
        CString details = CA2WEX<256>(e.what());
        CString error;
        VERIFY(error.LoadString(IDS_FAILED_TO_SAVE_SEARCH_RESULT));
        error += L" " + details;
        AfxMessageBox(error, MB_ICONERROR);
    }
}

void CAdBookDlg::SaveSearchFilter()
{
    SearchFilters sfl;
    const int itemCount = searchFilters_.GetItemCount();
    for (int i = 0; i < itemCount; ++i)
    {
        const auto itemData = static_cast<DWORD>(searchFilters_.GetItemData(i));
        const auto attrId = LOWORD(itemData);
        const auto matchingRule = static_cast<adbook::LdapRequest::MathingRule>(HIWORD(itemData));
        const CString val = searchFilters_.GetItemText(i, ValueColumnId);
        SearchFilter sf = {attrId, matchingRule, val};
        sfl.push_back(sf);
    }
    mws_.SetSearchFilters(sfl);
}

void CAdBookDlg::SaveWindowPosition()
{
    CRect rect;
    GetWindowRect(&rect);
    mws_.SetRect(rect);
}

void CAdBookDlg::RestoreWindowPosition()
{                 
    auto rect = mws_.GetRect();
    if (rect.Width() && rect.Height())
    {
        if (rect.left < 0 && rect.top < 0)
        {
            ShowWindow(SW_MAXIMIZE);
        }
        else
        {
            MoveWindow(rect.left, rect.top, rect.Width(), rect.Height());
        }        
    }
}
                  
BEGIN_MESSAGE_MAP(CAdBookDlg, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_COMMAND(ID_SELECT_PHOTO, OnSelectPhoto)
    ON_COMMAND(ID_CLEAR_PHOTO, OnClearPhoto)
ON_WM_SIZE()
ON_BN_CLICKED(IDC_BUTTON_ADD, &CAdBookDlg::OnBnClickedButtonAdd)
ON_BN_CLICKED(IDC_BUTTON_REMOVE, &CAdBookDlg::OnBnClickedButtonRemove)
ON_BN_CLICKED(IDC_BUTTON_FIND, &CAdBookDlg::OnBnClickedButtonFind)
ON_BN_CLICKED(IDC_BUTTON_SETTINGS, &CAdBookDlg::OnBnClickedButtonSettings)
ON_BN_CLICKED(IDCANCEL, &CAdBookDlg::OnBnClickedCancel)
ON_WM_GETMINMAXINFO()
ON_WM_CLOSE()
ON_CBN_EDITCHANGE(IDC_COMBO_TEXT, &CAdBookDlg::OnCbnEditchangeComboText)
ON_CBN_SELCHANGE(IDC_COMBO_TEXT, &CAdBookDlg::OnCbnSelchangeComboText)
ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_FILTERS, &CAdBookDlg::OnLvnItemchangedListFilters)
ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_RESULTS, &CAdBookDlg::OnLvnGetdispinfoListResults)
ON_MESSAGE(UM_REFRESH_TOTAL, &CAdBookDlg::OnUmRefreshTotal)
ON_MESSAGE(UM_START, &CAdBookDlg::OnUmStart)
ON_MESSAGE(UM_STOP, &CAdBookDlg::OnUmStop)
ON_NOTIFY(HDN_ITEMCLICK, 0, &CAdBookDlg::OnHdnItemclickListResults)
ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_RESULTS, &CAdBookDlg::OnLvnItemchangedListResults)
ON_NOTIFY(LVN_ITEMCHANGED, IDC_CONTACT_DETAILS1, &CAdBookDlg::OnLvnItemchangedContactDetails)
ON_BN_CLICKED(IDC_BUTTON_COPY1, &CAdBookDlg::OnBnClickedButtonCopy1)
ON_BN_CLICKED(IDC_BUTTON_CHANGE1, &CAdBookDlg::OnBnClickedButtonChange1)
ON_BN_CLICKED(IDC_BUTTON_COPY2, &CAdBookDlg::OnBnClickedButtonCopy2)
ON_BN_CLICKED(IDC_BUTTON_CHANGE2, &CAdBookDlg::OnBnClickedButtonChange2)
ON_BN_CLICKED(IDC_BUTTON_SELECT, &CAdBookDlg::OnBnClickedButtonSelect)
ON_NOTIFY(LVN_ITEMCHANGED, IDC_CONTACT_DETAILS2, &CAdBookDlg::OnLvnItemchangedContactDetails)
ON_NOTIFY(NM_DBLCLK, IDC_CONTACT_DETAILS1, &CAdBookDlg::OnNMDblclkContactDetails1)
ON_NOTIFY(NM_DBLCLK, IDC_CONTACT_DETAILS2, &CAdBookDlg::OnNMDblclkContactDetails2)
END_MESSAGE_MAP()


// CAdBookDlg message handlers

BOOL CAdBookDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // Add "About..." menu item to system menu.


    // IDM_ABOUTBOX must be in the system command range.
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != NULL)
    {
        BOOL bNameValid;
        CString strAboutMenu;
        bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
        ASSERT(bNameValid);
        if (!strAboutMenu.IsEmpty())
        {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }

    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not attributes dialog
    SetIcon(m_hIcon, TRUE);        // Set big icon
    SetIcon(m_hIcon, FALSE);       // Set small icon
    valList_.LimitText(200);

    /*
    adSearcher_.SetCallbacks(
        std::bind(&CAdBookDlg::OnNewItem, this, std::placeholders::_1),
        std::bind(&CAdBookDlg::OnStart, this),
        std::bind(&CAdBookDlg::OnStop, this));
        */
    // callstack is shorter when using lambdas
    adSearcher_.SetCallbacks(
        [this](adbook::AdPersonDesc && arg) { OnNewItem(std::move(arg));},
        [this]() { OnStart(); },
        [this]() { OnStop(); });
    
    InitAnchor();
    InitMinSize();
    InitAttrList();    
    InitConditionList();
    InitContactDetails();    
    InitSearchFiltersList();
    LoadSearchFilterStrings();
    InitSearchResultList();
    UpdateAddButtonState();
    UpdateRemoveButtonState();    
    RestoreWindowPosition();
    if (mws_.GetCondCombineOp() == MainWndSettings::ConditionsCombineOperation::And)
    {
        CheckDlgButton(IDC_RADIO_ALL, BST_CHECKED);        
    }
    else
    {
        CheckDlgButton(IDC_RADIO_ONE, BST_CHECKED);
    }    
    RefreshPersonButtons(LB_ERR);    
    return TRUE;  // return TRUE  unless you set the focus to attributes control
}


void CAdBookDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else
    {
        CDialogEx::OnSysCommand(nID, lParam);
    }
}

// If you add attributes minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CAdBookDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this); // device context for painting

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // Center icon in client rectangle
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // Draw the icon
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialogEx::OnPaint();
    }
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CAdBookDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

void CAdBookDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialogEx::OnSize(nType, cx, cy);
    wndAnchor_.OnSize();
    if (photo_.GetSafeHwnd())
    {
        photo_.Invalidate();
    }    
}

int CAdBookDlg::GetSelectedPersonIdx()
{
    const int selMark = searchResults_.GetSelectionMark();
    const int selCount = searchResults_.GetSelectedCount();
    if (LB_ERR!=selMark && 1==selCount)
    {
        return selMark;
    }
    return LB_ERR;
}

void CAdBookDlg::ResizeContactDetailsCols()
{
    contactDetails1_.SetColumnWidth(1, LVSCW_AUTOSIZE);
    contactDetails2_.SetColumnWidth(1, LVSCW_AUTOSIZE);
}

void CAdBookDlg::OnBnClickedButtonAdd()
{
    int curSel = attrList_.GetCurSel();
    VERIFY(LB_ERR != curSel);
    const auto attrId = static_cast<adbook::Attributes::AttrId>(attrList_.GetItemData(curSel));
    CString attrDisplayName;
    GetDlgItemText(IDC_COMBO_ATTR, attrDisplayName);

    curSel = condList_.GetCurSel();
    VERIFY(LB_ERR != curSel);
    const auto rule = static_cast<adbook::LdapRequest::MathingRule>(condList_.GetItemData(curSel));
    CString conditionDisplayName;
    GetDlgItemText(IDC_COMBO_CONDITION, conditionDisplayName);
    
    CString val;
    GetDlgItemText(IDC_COMBO_TEXT, val);
    VERIFY(!val.IsEmpty());
    
    LVFINDINFO fi = { LVFI_STRING, static_cast<LPCWSTR>(attrDisplayName) };
    int idx = searchFilters_.FindItem(&fi);
    if (-1 == idx)
    {
        idx = searchFilters_.InsertItem(searchFilters_.GetItemCount(), attrDisplayName);
    }
    else
    {
        searchFilters_.SetItemText(idx, 0, attrDisplayName);
    }
    searchFilters_.SetItemText(idx, ConditionColumnId, conditionDisplayName);
    searchFilters_.SetItemText(idx, ValueColumnId, val);
    const DWORD itemData = MAKELONG(attrId, rule);
    searchFilters_.SetItemData(idx, itemData);
}


void CAdBookDlg::OnBnClickedButtonRemove()
{
    const UINT selCount = searchFilters_.GetSelectedCount();    
    const int selMark = searchFilters_.GetSelectionMark();
    if (LB_ERR != selMark && 1 == selCount)
    {
        VERIFY(searchFilters_.DeleteItem(selMark));
    }
}

void CAdBookDlg::OnBnClickedButtonFind()
{
    try
    {
        CWaitCursor wc;
        if (adSearcher_.IsStarted())
        {
            adSearcher_.Stop();            
        }
        else
        {
            adSearcher_.Start(ConstructLdapRequest(), theApp.GetAppSettings().GetConnectionSettings());
            GetDlgItem(IDC_BUTTON_FIND)->EnableWindow(FALSE);   // disable 'find' button until the search thread is started.
            GetDlgItem(IDC_BUTTON_ADD)->EnableWindow(FALSE);
            GetDlgItem(IDC_BUTTON_REMOVE)->EnableWindow(FALSE);
            GetDlgItem(IDC_BUTTON_CHANGE1)->EnableWindow(FALSE);
            GetDlgItem(IDC_BUTTON_CHANGE2)->EnableWindow(FALSE);
            GetDlgItem(IDC_BUTTON_SELECT)->EnableWindow(FALSE);
            CleanSortArrow();
        }
    }
    catch (const adbook::Error & e)
    {        
        AfxMessageBox(e.What().c_str(), MB_ICONERROR);
    }
    catch (const std::exception &)
    {
        AfxMessageBox(IDS_UNKNOWN_ERROR_DURING_SEARCH, MB_ICONERROR);
    }
}

void CAdBookDlg::OnBnClickedButtonSettings()
{    
    CSettingsDlg dlg(theApp.GetAppSettings());
    if (IDOK == dlg.DoModal())
    {
        theApp.GetAppSettings() = dlg.GetAppSettings();
    }
}

void CAdBookDlg::OnBnClickedCancel()
{
    SaveSearchFilter();
    SaveSearchResults();
    SaveWindowPosition();
    SaveSearchFilterCols();
    SaveSearchResultCols();
    SaveSearchFilterStrings();
    if (IsDlgButtonChecked(IDC_RADIO_ALL) == BST_CHECKED)
    {
        mws_.SetCondCombineOp(MainWndSettings::ConditionsCombineOperation::And);
    }
    else
    {
        mws_.SetCondCombineOp(MainWndSettings::ConditionsCombineOperation::Or);
    }
    
    CWaitCursor wc;

    if (adSearcher_.IsStarted())
    {
        adSearcher_.Stop();
        try
        {
            adSearcher_.Wait();
        }
        catch (const std::exception&)
        { //-V565
            // user wants to exit. An error message should not be displayed.
        }
    }
    
    CDialogEx::OnCancel();                           
}

void CAdBookDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{    
    if (minSize_.cx && minSize_.cy)
    {
        lpMMI->ptMinTrackSize.x = minSize_.cx;
        lpMMI->ptMinTrackSize.y = minSize_.cy;
    }
    
    CDialogEx::OnGetMinMaxInfo(lpMMI);
}

void CAdBookDlg::OnClose()
{    
    
    CDialogEx::OnClose();
}

void CAdBookDlg::UpdateAddButtonState()
{
    CString s;
    GetDlgItemText(IDC_COMBO_TEXT, s);
    s.Trim();
    GetDlgItem(IDC_BUTTON_ADD)->EnableWindow(!s.IsEmpty() && !adSearcher_.IsStarted());
}

void CAdBookDlg::UpdateRemoveButtonState()
{
    const UINT selCount = searchFilters_.GetSelectedCount();    
    GetDlgItem(IDC_BUTTON_REMOVE)->EnableWindow(selCount != 0 && !adSearcher_.IsStarted());
}

void CAdBookDlg::OnLvnItemchangedListFilters(NMHDR * /*pNMHDR*/ , LRESULT *pResult)
{    
    UpdateRemoveButtonState();
    *pResult = 0;
}

adbook::LdapRequest CAdBookDlg::ConstructLdapRequest()
{
    adbook::LdapRequest lr;
    const int itemCount = searchFilters_.GetItemCount();
    for (int i = 0; i < itemCount; ++i)
    {
        const auto itemData = static_cast<DWORD>(searchFilters_.GetItemData(i));
        const auto id = static_cast<adbook::Attributes::AttrId>(LOWORD(itemData));
        const auto mr = static_cast<adbook::LdapRequest::MathingRule>(HIWORD(itemData));
        const CString val = searchFilters_.GetItemText(i, ValueColumnId);
        if (id == AllAttributes)
        {
            const auto attrNames = adbook::Attributes::GetInstance().GetLdapAttrNames();
            for (const auto & ii : attrNames)
            {
                lr.AddRule(ii, mr, static_cast<PCWSTR>(val));                
            }
            lr.AddOR();
        }
        else
        {
            lr.AddRule(id, mr, static_cast<PCWSTR>(val));
        }
    }
    if (itemCount != 0)
    {
        if (BST_CHECKED == IsDlgButtonChecked(IDC_RADIO_ALL))
        {
            lr.AddAND();
        }
        else
        {
            lr.AddOR();
        }
    }
    lr.AddRule(L"objectCategory", adbook::LdapRequest::ExactMatch, L"person");
    if (itemCount != 0)
    {
        lr.AddAND();
    }
    return lr;
}

LRESULT CAdBookDlg::OnUmRefreshTotal(WPARAM itemCount, LPARAM)
{
    searchResults_.SetItemCount(static_cast<int>(itemCount));
    CString s;
    VERIFY(s.LoadString(IDS_TOTAL));
    s.AppendFormat(L" %u", static_cast<unsigned int>(itemCount));
    SetDlgItemText(IDC_STATIC_TOTAL, s);    
    return 0;
}

void CAdBookDlg::OnNewItem(adbook::AdPersonDesc && item)
{    
    static time_t prevTime = time(nullptr);
    time_t curTime = time(nullptr);     //-V656
    {
        std::lock_guard<decltype(personsMutex_)> lg(personsMutex_);
        persons_.push_back(std::move(item));
    }
    if (curTime - prevTime > 1)
    {
        const auto itemCount = persons_.size();
        PostMessage(UM_REFRESH_TOTAL, itemCount);
        prevTime = curTime;
    }    
}

void CAdBookDlg::OnStart()  // called from another thread
{
    PostMessage(UM_START);  // redirect processing to the main(gui) thread
}

void CAdBookDlg::OnStop()   // called from another thread
{
    PostMessage(UM_STOP);   // redirect processing to the main(gui) thread
}

LRESULT CAdBookDlg::OnUmStart(WPARAM, LPARAM)
{    
    std::lock_guard<decltype(personsMutex_)> lg(personsMutex_);
    persons_.clear();

    CString s;
    VERIFY(s.LoadString(IDS_STOP));    
    SetDlgItemText(IDC_BUTTON_FIND, s);

    OnUmRefreshTotal(0, 0);
    GetDlgItem(IDC_BUTTON_FIND)->EnableWindow(TRUE);    
    return 0;
}

void CAdBookDlg::RememberSuccessfulSearchString()
{
    const int itemCount = searchFilters_.GetItemCount();
    for (int i = 0; i < itemCount; ++i)
    {
        const CString s = searchFilters_.GetItemText(i, ValueColumnId);
        if (CB_ERR == valList_.FindStringExact(0, s))
        {
            valList_.InsertString(0, s);
            if (valList_.GetCount() > static_cast<int>(valListMaxItemCount))
            {
                valList_.DeleteString(valList_.GetCount() - 1);
            }
        }
    }
}

LRESULT CAdBookDlg::OnUmStop(WPARAM, LPARAM)
{
    int itemCount = 0;
    std::lock_guard<decltype(personsMutex_)> lg(personsMutex_);
    itemCount = static_cast<int>(persons_.size());
    
    CString s;
    VERIFY(s.LoadString(IDS_FIND));
    SetDlgItemText(IDC_BUTTON_FIND, s);

    OnUmRefreshTotal(itemCount, 0);
    GetDlgItem(IDC_BUTTON_FIND)->EnableWindow(TRUE);

    if (itemCount)
    {        
        RememberSuccessfulSearchString();
    }
    try
    {
        adSearcher_.Wait();
        UpdateAddButtonState();
        UpdateRemoveButtonState();
        RefreshPersonButtons();
    }
    catch (const adbook::Error & e)
    {
        AfxMessageBox(e.What().c_str(), MB_ICONERROR);
    }
    catch (const std::exception & )
    {
        AfxMessageBox(IDS_UNKNOWN_ERROR_DURING_SEARCH, MB_ICONERROR);
    }
    return 0;
}

void CAdBookDlg::DisplayPersonDetails(const int personIdx)
{
    std::lock_guard<decltype(personsMutex_)> lg(personsMutex_);
    const auto & person = persons_.at(personIdx);

    auto f = [&person](CListCtrl & listCtrl)
    {
        int itemCount = listCtrl.GetItemCount();
        for (int i = 0; i < itemCount; ++i)
        {
            const auto ai = static_cast<adbook::Attributes::AttrId>(listCtrl.GetItemData(i));
            const auto attrName = adbook::Attributes::GetInstance().GetLdapAttrName(ai);
            if (person.IsAttributeSet(attrName))
            {                
                VERIFY(listCtrl.SetItemText(i, ContactDetailsAttrValueCol, person.GetStringAttr(attrName).c_str()));
            }
            else
            {
                VERIFY(listCtrl.SetItemText(i, ContactDetailsAttrValueCol, L""));
            }
        }
    };
    f(contactDetails1_);
    f(contactDetails2_);
    ResizeContactDetailsCols();
    const adbook::BinaryAttrVal bav = person.GetBinaryAttr(
        adbook::Attributes::GetInstance().GetLdapAttrName(
            adbook::Attributes::ThumbnailPhoto
        )
    );
    if (!bav.empty())
    {
        photo_.Load(bav);
    }
    else
    {
        photo_.Unload();
        photo_.Invalidate();
    }
}

void CAdBookDlg::RefreshPersonButtons(int personIdx, int attrIdx1, int attrIdx2)
{
    CWnd * copy1 = GetDlgItem(IDC_BUTTON_COPY1),
         * copy2 = GetDlgItem(IDC_BUTTON_COPY2);
    CWnd * edit1 = GetDlgItem(IDC_BUTTON_CHANGE1),
         * edit2 = GetDlgItem(IDC_BUTTON_CHANGE2),
         * selectPhoto = GetDlgItem(IDC_BUTTON_SELECT);
    VERIFY(edit1 && edit2 && copy1 && copy2 && selectPhoto);

    if (LB_ERR == personIdx)
    {
        personIdx = searchResults_.GetSelectionMark();
    }
    if (LB_ERR == personIdx)
    {
        copy1->EnableWindow(FALSE);
        copy2->EnableWindow(FALSE);
        edit1->EnableWindow(FALSE);
        edit2->EnableWindow(FALSE);
        selectPhoto->EnableWindow(FALSE);
        return;
    }
    if (LB_ERR == attrIdx1)
    {
        attrIdx1 = contactDetails1_.GetSelectionMark();
    }
    if (LB_ERR == attrIdx2)
    {
        attrIdx2 = contactDetails2_.GetSelectionMark();
    }
    auto getAttrId = [](CListCtrl & contactDetails, const int attrIdx) -> adbook::Attributes::AttrId
    {
        if (LB_ERR == attrIdx)
        {
            return adbook::Attributes::AttrIdEnd;
        }
        return static_cast<adbook::Attributes::AttrId>(contactDetails.GetItemData(attrIdx));
    };
    const auto id1 = (attrIdx1 == LB_ERR ? getAttrId(contactDetails1_, attrIdx1) : 
                                        static_cast<adbook::Attributes::AttrId>(contactDetails1_.GetItemData(attrIdx1)));
    const auto id2 = (attrIdx2 == LB_ERR ? getAttrId(contactDetails2_, attrIdx2) :
                                        static_cast<adbook::Attributes::AttrId>(contactDetails2_.GetItemData(attrIdx2)));
    const bool searchingIsRunning = adSearcher_.IsStarted();
    std::lock_guard<decltype(personsMutex_)> lg(personsMutex_);
    const auto & person = persons_.at(personIdx);    
    copy1->EnableWindow(adbook::Attributes::AttrIdEnd != id1 &&
        !contactDetails1_.GetItemText(attrIdx1, ContactDetailsAttrValueCol).IsEmpty());
    copy2->EnableWindow(adbook::Attributes::AttrIdEnd != id2 &&
        !contactDetails2_.GetItemText(attrIdx2, ContactDetailsAttrValueCol).IsEmpty());
    edit1->EnableWindow(person.IsAttributeWritable(id1) && !searchingIsRunning);
    edit2->EnableWindow(person.IsAttributeWritable(id2) && !searchingIsRunning);
    selectPhoto->EnableWindow(person.IsAttributeWritable(adbook::Attributes::ThumbnailPhoto));
}

void CAdBookDlg::OnLvnGetdispinfoListResults(NMHDR *pNMHDR, LRESULT *pResult)
{
    std::lock_guard<decltype(personsMutex_)> lg(personsMutex_);
    NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
    if (pDispInfo->item.mask & LVIF_TEXT)
    {
        const auto & person = persons_.at(pDispInfo->item.iItem);
        const auto id = searchResultsColIdx_.at(pDispInfo->item.iSubItem);
        const auto attrName = adbook::Attributes::GetInstance().GetLdapAttrName(id);
        if (person.IsAttributeSet(attrName))
        {            
            StringCchCopyW(pDispInfo->item.pszText, pDispInfo->item.cchTextMax, person.GetStringAttr(attrName).c_str());
        }        
    }
    *pResult = 0;
}

void CAdBookDlg::Sort(const int colId, const bool ascending)
{
    CWaitCursor wc;
    const auto id = searchResultsColIdx_.at(colId);
    const auto sortAttr = adbook::Attributes::GetInstance().GetLdapAttrName(id);
    auto pred = [&sortAttr, ascending](const adbook::AdPersonDesc & itemLeft, const adbook::AdPersonDesc & itemRight)
    {        
        const auto left = itemLeft.GetStringAttr(sortAttr), right = itemRight.GetStringAttr(sortAttr);
        return ascending ? left > right : right > left;
    };

    std::lock_guard<decltype(personsMutex_)> lg(personsMutex_);

    std::stable_sort(persons_.begin(), persons_.end(), pred);
    searchResults_.Invalidate();
    const int selPerson = GetSelectedPersonIdx();
    if (LB_ERR != selPerson)
    {
        DisplayPersonDetails(selPerson);
        RefreshPersonButtons(selPerson);
    }
}

void CAdBookDlg::OnHdnItemclickListResults(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
    CHeaderCtrl * headerCtrl = searchResults_.GetHeaderCtrl();

    bool sortAscending = true;
    for (int i = 0, count = headerCtrl->GetItemCount(); i < count; ++i)
    {
        HDITEM hdItem{ HDI_FORMAT };
        VERIFY(headerCtrl->GetItem(i, &hdItem));
        if (i == phdr->iItem)
        {
            if (hdItem.fmt & HDF_SORTDOWN)
            {
                hdItem.fmt &= ~HDF_SORTDOWN;
                hdItem.fmt |= HDF_SORTUP;
                sortAscending = false;
            }
            else
            {
                hdItem.fmt &= ~HDF_SORTUP;
                hdItem.fmt |= HDF_SORTDOWN;
            }
        }
        else
        {
            hdItem.fmt &= ~HDF_SORTDOWN;
            hdItem.fmt &= ~HDF_SORTUP;
        }
        VERIFY(headerCtrl->SetItem(i, &hdItem));
    }

    Sort(phdr->iItem, sortAscending);
    *pResult = 0;
}

void CAdBookDlg::OnLvnItemchangedListResults(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    *pResult = 0;

    if (pNMLV->iItem == -1)
    {
        return;
    }
    if (0 == (pNMLV->uChanged & LVIF_STATE))
    {
        return;
    }
    if (pNMLV->uNewState & LVIS_SELECTED && !(pNMLV->uOldState & LVIS_SELECTED))
    {
        DisplayPersonDetails(pNMLV->iItem);
        RefreshPersonButtons(pNMLV->iItem);
    }
}

void CAdBookDlg::OnLvnItemchangedContactDetails(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    *pResult = 0;

    if (pNMLV->iItem == -1)
    {
        return;
    }
    if (0 == (pNMLV->uChanged & LVIF_STATE))
    {
        return;
    }
    if (pNMLV->uNewState & LVIS_SELECTED && !(pNMLV->uOldState & LVIS_SELECTED))
    {
        const int selPerson = GetSelectedPersonIdx();
        if (LB_ERR != selPerson)
        {
            if (IDC_CONTACT_DETAILS1 == pNMLV->hdr.idFrom)
            {
                RefreshPersonButtons(selPerson, pNMLV->iItem);
            }
            else if (IDC_CONTACT_DETAILS2 == pNMLV->hdr.idFrom)
            {
                RefreshPersonButtons(selPerson, LB_ERR, pNMLV->iItem);
            }
        }
        else
        {
            RefreshPersonButtons(LB_ERR);
        }
    }
}

namespace
{
    void CopySelectedAttrValueToClipboard(CListCtrl & contactDetails, const int column)
    {
        const int selMark = contactDetails.GetSelectionMark();
        const UINT selCount = contactDetails.GetSelectedCount();
        if (LB_ERR==selMark || !selCount)
        {
            return;
        }
        const CString value = contactDetails.GetItemText(selMark, column);
        TextClipboard tc;
        tc.Put(value);
    }
}

void CAdBookDlg::OnBnClickedButtonCopy1()
{
    CopySelectedAttrValueToClipboard(contactDetails1_, ContactDetailsAttrValueCol);
}

void CAdBookDlg::OnBnClickedButtonCopy2()
{
    CopySelectedAttrValueToClipboard(contactDetails2_, ContactDetailsAttrValueCol);
}

void CAdBookDlg::ApplyNewValue(adbook::AdPersonDesc & person, const adbook::Attributes::AttrId id, const CString & newValue)
{
    const auto attrName = adbook::Attributes::GetInstance().GetLdapAttrName(id);
    auto & cs = theApp.GetAppSettings().GetConnectionSettings();
    adbook::AdConnector personConnector(cs, person.GetDn());
    personConnector.Connect();
        
    if (adbook::Attributes::CommonName == id)   // special handling for CN
    {
        if (newValue.IsEmpty())
        {
            HR_ERROR(E_INVALIDARG);
        }        
        personConnector.Rename(static_cast<PCWSTR>(newValue));
        auto dn = personConnector.DownloadStringAttr(adbook::AdAttrDn);
        person.SetStringAttr(adbook::AdAttrDn, dn);
    }
    else
    {
        CWaitCursor wc;
        personConnector.UploadStringAttr(attrName, static_cast<PCWSTR>(newValue));
    }
    person.SetStringAttr(attrName, newValue);
}

void CAdBookDlg::DisplaySvAttrEditor(CListCtrl & personDetails)
{
    const int selMark = personDetails.GetSelectionMark();
    const int selCount = personDetails.GetSelectedCount();
    if (LB_ERR ==selMark || 1 != selCount)
    {
        return;
    }
    const auto aid = static_cast<adbook::Attributes::AttrId>(personDetails.GetItemData(selMark));
    if (!adbook::Attributes::GetInstance().IsAttrSupported(aid))
    {
        return;
    }
    const int selPerson = GetSelectedPersonIdx();
    if (LB_ERR == selPerson)
    {
        return;
    }
    std::lock_guard<decltype(personsMutex_)> lg(personsMutex_);
    auto & person = persons_.at(selPerson);
    SvAttrEditor sae(person, aid, this);
    if (sae.DoModal() != IDOK)
    {
        return;
    }
    const auto newValue = sae.GetNewValue();
    if (adbook::Attributes::CommonName == aid && newValue.IsEmpty())
    {
        AfxMessageBox(IDS_EMPTY_CN_ERROR, MB_ICONERROR);
        return;        
    }
    try
    {
        ApplyNewValue(person, aid, newValue);
    }
    catch (const adbook::Error & e)
    {
        AfxMessageBox(e.What().c_str(), MB_ICONERROR);
    }
    catch (const std::exception &)
    {
        AfxMessageBox(IDS_UNKNOWN_ERROR, MB_ICONERROR);
    }
    DisplayPersonDetails(selPerson);
    searchResults_.RedrawItems(selPerson, selPerson);    
}

void CAdBookDlg::OnBnClickedButtonChange1()
{
    DisplaySvAttrEditor(contactDetails1_);
}

void CAdBookDlg::OnBnClickedButtonChange2()
{
    DisplaySvAttrEditor(contactDetails2_);
}

void CAdBookDlg::OnSelectPhoto()
{
    CString filter;
    VERIFY(filter.LoadString(IDS_SELECT_PHOTO_DLG_FILTER));
    CFileDialog fd(TRUE, nullptr, nullptr, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST, filter, this);
    const auto photoMaxSizeInBytes = 
        adbook::Attributes::GetInstance().GetAttrMaxLength(adbook::Attributes::ThumbnailPhoto);
    CString title;
    title.Format(IDS_OPEN_PHOTO_DLG_TITLE, photoMaxSizeInBytes / 1024);
    VERIFY(!title.IsEmpty());
    const std::wstring titleBuf = static_cast<PCWSTR>(title);
    fd.m_ofn.lpstrTitle = &titleBuf.at(0);
    if (fd.DoModal() != IDOK)
    {
        return;
    }
    const int selPerson = GetSelectedPersonIdx();
    if (LB_ERR == selPerson)
    {
        return;
    }
    CFile photoFile;
    if (!photoFile.Open(fd.GetPathName(), CFile::modeRead))
    {
        AfxMessageBox(IDS_FAILED_TO_READ_A_FILE, MB_ICONERROR);
        return;
    }
    const ULONGLONG fileSize = photoFile.GetLength();
    if (0 == fileSize)
    {
        AfxMessageBox(IDS_FILE_IS_EMPTY, MB_ICONERROR);
        return;
    }
    if (fileSize > photoMaxSizeInBytes)
    {
        AfxMessageBox(IDS_FILE_IS_TOO_LARGE, MB_ICONERROR);
        return;
    }
    adbook::BinaryAttrVal photoBuf(static_cast<size_t>(fileSize));
    if (fileSize != photoFile.Read(&photoBuf[0], static_cast<UINT>(fileSize)))
    {
        AfxMessageBox(IDS_FAILED_TO_READ_A_FILE, MB_ICONERROR);
        return;
    }
    std::lock_guard<decltype(personsMutex_)> lg(personsMutex_);
    auto & person = persons_.at(selPerson);
    const auto dn = person.GetDn();
    const auto attrName = adbook::Attributes::GetInstance().GetLdapAttrName(adbook::Attributes::ThumbnailPhoto);
    CString confirm;
    confirm.Format(IDS_PHOTO_UPLOAD_CONFIRM, dn.c_str(), attrName.c_str());
    if (IDOK != AfxMessageBox(confirm, MB_ICONQUESTION | MB_OKCANCEL))
    {
        return;
    }
    adbook::AdConnector ac{ theApp.GetAppSettings().GetConnectionSettings(), dn };
    try
    {
        CWaitCursor wc;
        ac.Connect();        
        ac.UploadBinaryAttr(attrName, photoBuf);
        person.SetBinaryAttr(attrName, photoBuf);
        DisplayPersonDetails(selPerson);
    }
    catch (const adbook::Error & e)
    {
        AfxMessageBox(e.What().c_str(), MB_ICONERROR);
    }
    catch (const std::exception &)
    {
        AfxMessageBox(IDS_UNKNOWN_ERROR, MB_ICONERROR);
    }
}

void CAdBookDlg::OnClearPhoto()
{
    const int selPerson = GetSelectedPersonIdx();
    if (LB_ERR == selPerson)
    {
        return;
    }
    std::lock_guard<decltype(personsMutex_)> lg(personsMutex_);
    auto & person = persons_.at(selPerson);
    const auto dn = person.GetDn();
    adbook::AdConnector ac{ theApp.GetAppSettings().GetConnectionSettings(), dn };
    try
    {
        CWaitCursor wc;
        ac.Connect();
        const auto attrName = adbook::Attributes::GetInstance().GetLdapAttrName(adbook::Attributes::ThumbnailPhoto);
        ac.UploadBinaryAttr(attrName, adbook::BinaryAttrVal());
        person.SetBinaryAttr(attrName, adbook::BinaryAttrVal());
        DisplayPersonDetails(selPerson);
    }
    catch (const adbook::Error & e)
    {
        AfxMessageBox(e.What().c_str(), MB_ICONERROR);
    }
    catch (const std::exception &)
    {
        AfxMessageBox(IDS_UNKNOWN_ERROR, MB_ICONERROR);
    }
}

void CAdBookDlg::OnBnClickedButtonSelect()
{    
     POINT pt;
     VERIFY(GetCursorPos(&pt));     
     HMENU hm = LoadMenu(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_MENU_PHOTO));
     VERIFY(hm);
     HMENU hpopup = GetSubMenu(hm, 0);
     VERIFY(SetForegroundWindow());
     VERIFY(TrackPopupMenu(hpopup, TPM_LEFTBUTTON | TPM_RIGHTALIGN, pt.x, pt.y, 0, GetSafeHwnd(), NULL));
     SendMessage(WM_NULL, 0, 0);     
     VERIFY(DestroyMenu(hpopup));
     VERIFY(DestroyMenu(hm));
}

void CAdBookDlg::OnCbnEditchangeComboText()
{
    UpdateAddButtonState();
}

void CAdBookDlg::OnCbnSelchangeComboText()
{        
    const int curSel = valList_.GetCurSel();
    if (CB_ERR != curSel)
    {
        CString text;
        valList_.GetLBText(curSel, text);
        text.Trim();
        GetDlgItem(IDC_BUTTON_ADD)->EnableWindow(!text.IsEmpty());
    }    
}

void CAdBookDlg::OnNMDblclkContactDetails1(NMHDR * /*pNMHDR*/ , LRESULT *pResult)
{
    if (GetDlgItem(IDC_BUTTON_CHANGE1)->IsWindowEnabled())
    {
        DisplaySvAttrEditor(contactDetails1_);
    }    
    *pResult = 0;
}

void CAdBookDlg::OnNMDblclkContactDetails2(NMHDR * /*pNMHDR*/ , LRESULT *pResult)
{    
    if (GetDlgItem(IDC_BUTTON_CHANGE2)->IsWindowEnabled())
    {
        DisplaySvAttrEditor(contactDetails2_);
    }    
    *pResult = 0;
}
