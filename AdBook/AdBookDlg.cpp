// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
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

CAdBookDlg::CAdBookDlg(
    std::shared_ptr<adbook::AbstractAdAccessFactory> adFactory,
    CWnd* pParent /*=NULL*/
) //-V730
    : CDialogEx(CAdBookDlg::IDD, pParent),
    _adFactory{adFactory}
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    _adPersonKeeper = _adFactory->GetAdPersonDescKeeper();
}

void CAdBookDlg::DoDataExchange(CDataExchange * pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_ATTR, _attrList);
    DDX_Control(pDX, IDC_COMBO_CONDITION, _condList);
    DDX_Control(pDX, IDC_COMBO_TEXT, _valList);
    DDX_Control(pDX, IDC_LIST_FILTERS, _searchFilters);
    DDX_Control(pDX, IDC_LIST_RESULTS, _searchResults);
    DDX_Control(pDX, IDC_CONTACT_DETAILS1, _contactDetails1);
    DDX_Control(pDX, IDC_CONTACT_DETAILS2, _contactDetails2);
    DDX_Control(pDX, IDC_STATIC_PHOTO, _photo);
}

void CAdBookDlg::InitAttrList()
{
    CString anyAttrUiName;
    VERIFY(anyAttrUiName.LoadStringW(IDS_ATTR_ANY));
    VERIFY(_attrList.SetItemData(_attrList.AddString(anyAttrUiName), AllAttributes) != CB_ERR);
    auto & attributes = adbook::Attributes::GetInstance();
    const auto ids = attributes.GetAttrIds();
    for (const auto i : ids)
    {
        if (attributes.IsEditableString(i))
        {
            const auto displayName = attributes.GetUiAttrName(i);
            VERIFY(_attrList.SetItemData(_attrList.AddString(displayName.c_str()), i) != CB_ERR);
        }
    }
    _attrList.SetCurSel(0);
}

void CAdBookDlg::InitConditionList()
{
    CString ruleName;
    VERIFY(ruleName.LoadString(IDS_MATCH_RULE_CONTAINS));
    VERIFY(_condList.SetItemData(_condList.AddString(ruleName), adbook::LdapRequestBuilder::Contains) != CB_ERR);
    VERIFY(ruleName.LoadString(IDS_MATCH_RULE_START_WITH));
    VERIFY(_condList.SetItemData(_condList.AddString(ruleName), adbook::LdapRequestBuilder::BeginWith) != CB_ERR);
    VERIFY(ruleName.LoadString(IDS_MATCH_RULE_EXACT_MATCH));
    VERIFY(_condList.SetItemData(_condList.AddString(ruleName), adbook::LdapRequestBuilder::ExactMatch) != CB_ERR);
    VERIFY(ruleName.LoadString(IDS_MATCH_RULE_END_WITH));
    VERIFY(_condList.SetItemData(_condList.AddString(ruleName), adbook::LdapRequestBuilder::EndWith) != CB_ERR);
    _condList.SetCurSel(0);
}

void CAdBookDlg::InitSearchResultList()
{
    _searchResults.SetExtendedStyle(_searchResults.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER|
        LVS_EX_HEADERDRAGDROP | LVS_EX_LABELTIP);

    const int numCols = _attrList.GetCount();
    for (int i = 0, colIdx=0; i < numCols; ++i)
    {
        CString title;
        _attrList.GetLBText(i, title);
        const auto id = static_cast<adbook::Attributes::AttrId>(_attrList.GetItemData(i));
        auto & attributes = adbook::Attributes::GetInstance();
        if (!attributes.IsAttrSupported(id))
        {
            continue;
        }
        int reasonableDefaultColWidthInPixels = 100;
        _searchResults.InsertColumn(colIdx, title, LVCFMT_LEFT, reasonableDefaultColWidthInPixels);
        _searchResultsColIdx[colIdx] = id;
        ++colIdx;
    }

    _searchResults.SetColumnWidth(_searchResults.GetHeaderCtrl()->GetItemCount()-1, LVSCW_AUTOSIZE_USEHEADER);

    auto rcw = _appSettings.GetMainWndSettings().GetResultColWidth();
    const int colCount = _searchResults.GetHeaderCtrl()->GetItemCount();
    if (static_cast<int>(rcw.size()) == colCount)
    {
        for (int i = 0; i < colCount; ++i)
        {
            if (rcw[i])
            {
                _searchResults.SetColumnWidth(i, rcw[i]);
            }
        }
    }
    auto rco = _appSettings.GetMainWndSettings().GetResultColOrder();
    if (!rco.empty() && static_cast<int>(rco.size()) == colCount)
    {
        VERIFY(_searchResults.SetColumnOrderArray(static_cast<int>(rco.size()), &rco[0]));
    }
    size_t numPeople = 0;
    try
    {
        std::lock_guard<decltype(_peopleMutex)> lk(_peopleMutex);
        _adPersonKeeper->SetNameByConnectionParams(_appSettings.GetConnectionParams());
        _adPersonKeeper->Load(_people);
        numPeople = _people.size();
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
    OnUmRefreshTotal(numPeople, 0);
}

void CAdBookDlg::InitMinSize()
{
    CRect rc;
    GetWindowRect(&rc);
    _minSize.cx = rc.right - rc.left;
    _minSize.cy = rc.bottom - rc.top;
}

void CAdBookDlg::InitContactDetails()
{
    _contactDetails1.SetExtendedStyle(_contactDetails1.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER |
        LVS_EX_LABELTIP);
    _contactDetails1.InsertColumn(ContactDetailsAttrNameCol, L"", LVCFMT_LEFT, LVSCW_AUTOSIZE);
    _contactDetails1.InsertColumn(ContactDetailsAttrValueCol, L"", LVCFMT_LEFT, LVSCW_AUTOSIZE);

    _contactDetails2.SetExtendedStyle(_contactDetails2.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER |
        LVS_EX_LABELTIP);
    _contactDetails2.InsertColumn(ContactDetailsAttrNameCol, L"", LVCFMT_LEFT, LVSCW_AUTOSIZE);
    _contactDetails2.InsertColumn(ContactDetailsAttrValueCol, L"", LVCFMT_LEFT, LVSCW_AUTOSIZE);
    auto & attributes = adbook::Attributes::GetInstance();
    const auto attrIds = attributes.GetAttrIds();
    for (size_t i = 0, num = attrIds.size(); i < num; ++i)
    {
        if (!attributes.IsEditableString(attrIds[i]))
        {
            continue;
        }
        const auto attrDisplayName = attributes.GetUiAttrName(attrIds[i]);
        if (i % 2 == 0)
        {
            const int idx = _contactDetails1.InsertItem(_contactDetails1.GetItemCount(), attrDisplayName.c_str());
            _contactDetails1.SetItemData(idx, attrIds[i]);
        }
        else
        {
            const int idx = _contactDetails2.InsertItem(_contactDetails2.GetItemCount(), attrDisplayName.c_str());
            _contactDetails2.SetItemData(idx, attrIds[i]);
        }
    }
    _contactDetails1.SetColumnWidth(0, LVSCW_AUTOSIZE);
    _contactDetails2.SetColumnWidth(0, LVSCW_AUTOSIZE);
}

void CAdBookDlg::InitAnchor()
{
    _wndAnchor.Init(GetSafeHwnd());
    _wndAnchor.Add(IDOK, WindowAnchor::BottomRight);
    _wndAnchor.Add(IDCANCEL, WindowAnchor::BottomRight);
    _wndAnchor.Add(IDC_BUTTON_ADD, WindowAnchor::TopRight);
    _wndAnchor.Add(IDC_BUTTON_FIND, WindowAnchor::TopRight);
    _wndAnchor.Add(IDC_BUTTON_REMOVE, WindowAnchor::TopRight);
    _wndAnchor.Add(IDC_BUTTON_SETTINGS, WindowAnchor::BottomLeft);
    _wndAnchor.Add(IDC_COMBO_TEXT, WindowAnchor::TopLeft | WindowAnchor::Right);
    _wndAnchor.Add(IDC_LIST_FILTERS, WindowAnchor::TopLeft | WindowAnchor::BottomRight, 1.0f, 1.0f, 1.0f, 0.2f);
    _wndAnchor.Add(IDC_CONTACT_DETAILS_CAPTION, WindowAnchor::BottomLeft, 1.0f, 1.0f, 1.0f, 0.8f);
    _wndAnchor.Add(IDC_CONTACT_DETAILS1, WindowAnchor::TopLeft | WindowAnchor::BottomRight, 1.0f, 0.4f, 0.2f, 1.0f);
    _wndAnchor.Add(IDC_CONTACT_DETAILS2, WindowAnchor::TopLeft | WindowAnchor::BottomRight, 0.6f, 0.8f, 0.2f, 1.0f);
    _wndAnchor.Add(IDC_BUTTON_COPY1, WindowAnchor::BottomRight, 1.0f, 0.4f, 1.0f, 1.0f);
    _wndAnchor.Add(IDC_BUTTON_CHANGE1, WindowAnchor::BottomRight, 1.0f, 0.4f, 1.0f, 1.0f);
    _wndAnchor.Add(IDC_BUTTON_COPY2, WindowAnchor::BottomRight, 1.0f, 0.8f, 1.0f, 1.0f);
    _wndAnchor.Add(IDC_BUTTON_CHANGE2, WindowAnchor::BottomRight, 1.0f, 0.8f, 1.0f, 1.0f);
    _wndAnchor.Add(IDC_BUTTON_SELECT, WindowAnchor::BottomRight, 1.0f, 1.0f, 1.0f, 1.0f);
    _wndAnchor.Add(IDC_STATIC_PHOTO, WindowAnchor::TopLeft | WindowAnchor::BottomRight, 0.2f, 1.0f, 0.2f, 1.0f);
    _wndAnchor.Add(IDC_PHOTO_CAPTION, WindowAnchor::Left | WindowAnchor::BottomRight, 0.2f, 1.0f, 1.0f, 0.8f);
    _wndAnchor.Add(IDC_LIST_RESULTS, WindowAnchor::TopLeft | WindowAnchor::BottomRight, 1.0f, 1.0f, 0.8f, 0.8f);
    _wndAnchor.Add(IDC_RADIO_ONE, WindowAnchor::TopLeft | WindowAnchor::Bottom, 1.0f, 1.0f, 0.8f, 0.2f);
    _wndAnchor.Add(IDC_RADIO_ALL, WindowAnchor::TopLeft | WindowAnchor::Bottom, 1.0f, 1.0f, 0.8f, 0.2f);
    _wndAnchor.Add(IDC_RESULT_CAPTION, WindowAnchor::TopLeft | WindowAnchor::BottomRight, 1.0f, 1.0f, 0.8f, 0.2f);
    _wndAnchor.Add(IDC_STATIC_TOTAL, WindowAnchor::Top | WindowAnchor::BottomRight, 1.0f, 1.0f, 0.8f, 0.2f);
}

void CAdBookDlg::InitSearchFiltersList()
{
    _searchFilters.SetExtendedStyle(_searchFilters.GetExtendedStyle() | LVS_EX_FULLROWSELECT);

    CString colTitle;
    VERIFY(colTitle.LoadStringW(IDS_COL_TITLE_ATTR));
    _searchFilters.InsertColumn(AttrColumnId, colTitle, LVCFMT_LEFT, 100);

    VERIFY(colTitle.LoadStringW(IDS_COL_TITLE_CONDITION));
    _searchFilters.InsertColumn(ConditionColumnId, colTitle, LVCFMT_LEFT, 100);

    VERIFY(colTitle.LoadStringW(IDS_COL_TITLE_VALUE));
    _searchFilters.InsertColumn(ValueColumnId, colTitle, LVCFMT_LEFT, 100);

    VERIFY(_searchFilters.SetColumnWidth(ValueColumnId, LVSCW_AUTOSIZE_USEHEADER));

    const auto fcw = _appSettings.GetMainWndSettings().GetFilterColWidth();
    const int colCount = _searchFilters.GetHeaderCtrl()->GetItemCount();
    if (static_cast<int>(fcw.size()) == colCount)
    {
        for (int i = 0; i < colCount; ++i)
        {
            if (fcw[i])
            {
                _searchFilters.SetColumnWidth(i, fcw[i]);
            }
        }
    }

    std::map<DWORD_PTR, CString> matchingRuleNames;
    for (int i = 0, max = _condList.GetCount(); i < max; ++i)
    {
        DWORD_PTR data = _condList.GetItemData(i);
        CString displayName;
        _condList.GetLBText(i, displayName);
        matchingRuleNames[data] = displayName;
    }
    auto & attributes = adbook::Attributes::GetInstance();
    for (const auto & i : _appSettings.GetMainWndSettings().GetSearchFilters())
    {
        int idx=0;
        auto attrId = static_cast<adbook::Attributes::AttrId>(i.attrId);
        if (attributes.IsAttrSupported(attrId))
        {
            idx = _searchFilters.InsertItem(_searchFilters.GetItemCount(), attributes.GetUiAttrName(attrId).c_str());
        }
        else
        {
            CString attrAny;
            VERIFY(attrAny.LoadStringW(IDS_ATTR_ANY));
            idx = _searchFilters.InsertItem(_searchFilters.GetItemCount(), attrAny);
        }
        _searchFilters.SetItemText(idx, ConditionColumnId, matchingRuleNames.at(i.rule));
        _searchFilters.SetItemText(idx, ValueColumnId, i.attrValue);
        const DWORD itemData = MAKELONG(i.attrId, i.rule);
        _searchFilters.SetItemData(idx, itemData);
    }
}

void CAdBookDlg::SaveSearchFilterCols()
{
    MainWndSettings::IntVector fcw;
    const int colCount = _searchFilters.GetHeaderCtrl()->GetItemCount();
    for (int i = 0; i < colCount; ++i)
    {
        const int colWidth = _searchFilters.GetColumnWidth(i);
        fcw.push_back(colWidth);
    }
    _appSettings.GetMainWndSettings().SetFilterColWidth(std::move(fcw));
}

void CAdBookDlg::SaveSearchFilterStrings()
{
    MainWndSettings::SearchFilterStrings sfs;
    const int count = _valList.GetCount();
    for (int i = 0; i < count; ++i)
    {
        CString s;
        _valList.GetLBText(i, s);
        s.Trim();
        if (!s.IsEmpty())
        {
            sfs.push_back(s);
        }
    }
    _appSettings.GetMainWndSettings().SetSearchFilterStrings(std::move(sfs));
}

void CAdBookDlg::LoadSearchFilterStrings()
{
    const auto & sfs = _appSettings.GetMainWndSettings().GetSearchFilterStrings();
    for (const auto & i : sfs)
    {
        _valList.InsertString(_valList.GetCount(), i);
    }
}

void CAdBookDlg::SaveSearchResultCols()
{
    MainWndSettings::IntVector rcw;
    const int colCount = _searchResults.GetHeaderCtrl()->GetItemCount();
    for (int i = 0; i < colCount; ++i)
    {
        const int colWidth = _searchResults.GetColumnWidth(i);
        rcw.push_back(colWidth);
    }
    _appSettings.GetMainWndSettings().SetResultColWidth(std::move(rcw));

    if (colCount)
    {
        MainWndSettings::IntVector rco(static_cast<size_t>(colCount));
        VERIFY(_searchResults.GetColumnOrderArray(&rco[0], static_cast<int>(rco.size())));
        _appSettings.GetMainWndSettings().SetResultColOrder(std::move(rco));
    }
}

void CAdBookDlg::CleanSortArrow()
{
    CHeaderCtrl * headerCtrl = _searchResults.GetHeaderCtrl();
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
        std::lock_guard<decltype(_peopleMutex)> lg(_peopleMutex);
        _adPersonKeeper->Save(_people);
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
    std::list<SearchFilter> sfl;
    const int itemCount = _searchFilters.GetItemCount();
    for (int i = 0; i < itemCount; ++i)
    {
        const auto itemData = static_cast<DWORD>(_searchFilters.GetItemData(i));
        const auto attrId = LOWORD(itemData);
        const auto matchingRule = static_cast<adbook::LdapRequestBuilder::MatchingRule>(HIWORD(itemData));
        const CString val = _searchFilters.GetItemText(i, ValueColumnId);
        SearchFilter sf = {attrId, matchingRule, val};
        sfl.push_back(sf);
    }
    _appSettings.GetMainWndSettings().SetSearchFilters(sfl);
}

void CAdBookDlg::SaveWindowPosition()
{
    CRect rect;
    GetWindowRect(&rect);
    _appSettings.GetMainWndSettings().SetRect(rect);
}

void CAdBookDlg::RestoreWindowPosition()
{
    auto rect = _appSettings.GetMainWndSettings().GetRect();
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
    _valList.LimitText(200);

    _adSearcher = _adFactory->CreateSearcher();

    // callstack is shorter when using lambdas
    _adSearcher->SetCallbacks(
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
    if (_appSettings.GetMainWndSettings().GetCondCombineOp() == MainWndSettings::ConditionsCombineOperation::And)
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
    _wndAnchor.OnSize();
    if (_photo.GetSafeHwnd())
    {
        _photo.Invalidate();
    }
}

int CAdBookDlg::GetSelectedPersonIdx()
{
    const int selMark = _searchResults.GetSelectionMark();
    const int selCount = _searchResults.GetSelectedCount();
    if (LB_ERR!=selMark && 1==selCount)
    {
        return selMark;
    }
    return LB_ERR;
}

void CAdBookDlg::ResizeContactDetailsCols()
{
    _contactDetails1.SetColumnWidth(1, LVSCW_AUTOSIZE);
    _contactDetails2.SetColumnWidth(1, LVSCW_AUTOSIZE);
}

void CAdBookDlg::OnBnClickedButtonAdd()
{
    int curSel = _attrList.GetCurSel();
    VERIFY(LB_ERR != curSel);
    const auto attrId = static_cast<adbook::Attributes::AttrId>(_attrList.GetItemData(curSel));
    CString attrDisplayName;
    GetDlgItemText(IDC_COMBO_ATTR, attrDisplayName);

    curSel = _condList.GetCurSel();
    VERIFY(LB_ERR != curSel);
    const auto rule = static_cast<adbook::LdapRequestBuilder::MatchingRule>(_condList.GetItemData(curSel));
    CString conditionDisplayName;
    GetDlgItemText(IDC_COMBO_CONDITION, conditionDisplayName);

    CString val;
    GetDlgItemText(IDC_COMBO_TEXT, val);
    VERIFY(!val.IsEmpty());

    for (int i = 0, numItems = _searchFilters.GetItemCount(); i < numItems; ++i)
    {
        const auto itemData = static_cast<DWORD>(_searchFilters.GetItemData(i));
        const auto lattrId = static_cast<adbook::Attributes::AttrId>(LOWORD(itemData));
        const auto lrule = static_cast<adbook::LdapRequestBuilder::MatchingRule>(HIWORD(itemData));
        const CString lval = _searchFilters.GetItemText(i, ValueColumnId);
        if (lattrId == attrId && lrule == rule && !lval.CompareNoCase(val)) {
            // duplicate found.
            return;
        }
    }

    const int idx = _searchFilters.InsertItem(_searchFilters.GetItemCount(), attrDisplayName);
    VERIFY(_searchFilters.SetItemText(idx, ConditionColumnId, conditionDisplayName));
    VERIFY(_searchFilters.SetItemText(idx, ValueColumnId, val));
    const DWORD itemData = MAKELONG(attrId, rule);
    VERIFY(_searchFilters.SetItemData(idx, itemData));
}


void CAdBookDlg::OnBnClickedButtonRemove()
{
    const UINT selCount = _searchFilters.GetSelectedCount();
    const int selMark = _searchFilters.GetSelectionMark();
    if (LB_ERR != selMark && 1 == selCount)
    {
        VERIFY(_searchFilters.DeleteItem(selMark));
    }
}

void CAdBookDlg::OnBnClickedButtonFind()
{
    try
    {
        CWaitCursor wc;
        if (_adSearcher->IsStarted())
        {
            _adSearcher->Stop();
        }
        else
        {
            auto connector = _adFactory->CreateConnector();
            connector->Connect(_appSettings.GetConnectionParams());
            connector->Disconnect();

            _adSearcher->Start(ConstructLdapRequest(), _appSettings.GetConnectionParams());
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
        AfxMessageBox(e.What(), MB_ICONERROR);
    }
    catch (const std::exception &)
    {
        AfxMessageBox(IDS_UNKNOWN_ERROR_DURING_SEARCH, MB_ICONERROR);
    }
}

void CAdBookDlg::OnBnClickedButtonSettings()
{
    CSettingsDlg dlg{ _adFactory, _appSettings.GetConnectionParams(), this };
    if (IDOK == dlg.DoModal())
    {
        _appSettings.GetConnectionParams() = dlg.GetConnectionParams();
        _adPersonKeeper->SetNameByConnectionParams(_appSettings.GetConnectionParams());
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
        _appSettings.GetMainWndSettings().SetCondCombineOp(MainWndSettings::ConditionsCombineOperation::And);
    }
    else
    {
        _appSettings.GetMainWndSettings().SetCondCombineOp(MainWndSettings::ConditionsCombineOperation::Or);
    }

    CWaitCursor wc;

    if (_adSearcher->IsStarted())
    {
        _adSearcher->Stop();
        try
        {
            _adSearcher->Wait();
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
    if (_minSize.cx && _minSize.cy)
    {
        lpMMI->ptMinTrackSize.x = _minSize.cx;
        lpMMI->ptMinTrackSize.y = _minSize.cy;
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
    GetDlgItem(IDC_BUTTON_ADD)->EnableWindow(!s.IsEmpty() && !_adSearcher->IsStarted());
}

void CAdBookDlg::UpdateRemoveButtonState()
{
    const UINT selCount = _searchFilters.GetSelectedCount();
    GetDlgItem(IDC_BUTTON_REMOVE)->EnableWindow(selCount != 0 && !_adSearcher->IsStarted());
}

void CAdBookDlg::OnLvnItemchangedListFilters(NMHDR * /*pNMHDR*/ , LRESULT *pResult)
{
    UpdateRemoveButtonState();
    *pResult = 0;
}

std::wstring CAdBookDlg::ConstructLdapRequest()
{
    auto & attributes = adbook::Attributes::GetInstance();
    adbook::LdapRequestBuilder lr;
    const int itemCount = _searchFilters.GetItemCount();

    for (int i = 0; i < itemCount; ++i)
    {
        const auto itemData = static_cast<DWORD>(_searchFilters.GetItemData(i));
        const auto id = static_cast<adbook::Attributes::AttrId>(LOWORD(itemData));
        if (id == AllAttributes)
        {
            const auto mr = static_cast<adbook::LdapRequestBuilder::MatchingRule>(HIWORD(itemData));
            const CString val = _searchFilters.GetItemText(i, ValueColumnId);
            const auto attrNames = attributes.GetLdapAttrNames();
            for (const auto & ii : attrNames)
            {
                if (attributes.IsEditableString(attributes.GetAttrId(ii.c_str()))) {
                    lr.AddRule(ii, mr, static_cast<PCWSTR>(val));
                }
            }
            lr.AddOR();
        }
    }
    for (int i = 0; i < itemCount; ++i)
    {
        const auto itemData = static_cast<DWORD>(_searchFilters.GetItemData(i));
        const auto id = static_cast<adbook::Attributes::AttrId>(LOWORD(itemData));
        if (id != AllAttributes)
        {
            const auto mr = static_cast<adbook::LdapRequestBuilder::MatchingRule>(HIWORD(itemData));
            const CString val = _searchFilters.GetItemText(i, ValueColumnId);
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
    lr.AddRule(L"objectCategory", adbook::LdapRequestBuilder::ExactMatch, L"person");
    if (itemCount != 0)
    {
        lr.AddAND();
    }
    return lr.Get();
}

LRESULT CAdBookDlg::OnUmRefreshTotal(WPARAM itemCount, LPARAM)
{
    _searchResults.SetItemCount(static_cast<int>(itemCount));
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
        std::lock_guard<decltype(_peopleMutex)> lg(_peopleMutex);
        _people.push_back(std::move(item));
    }
    if (curTime - prevTime > 1)
    {
        const auto itemCount = _people.size();
        PostMessage(UM_REFRESH_TOTAL, itemCount);
        prevTime = curTime;
    }
}

void CAdBookDlg::OnStart()  // called from another thread
{
    {
        std::lock_guard<decltype(_peopleMutex)> lg(_peopleMutex);
        _people.clear();
    }

    PostMessage(UM_START);  // redirect processing to the main(gui) thread
}

void CAdBookDlg::OnStop()   // called from another thread
{
    PostMessage(UM_STOP);   // redirect processing to the main(gui) thread
}

LRESULT CAdBookDlg::OnUmStart(WPARAM, LPARAM)
{

    CString s;
    VERIFY(s.LoadString(IDS_STOP));
    SetDlgItemText(IDC_BUTTON_FIND, s);

    OnUmRefreshTotal(0, 0);
    GetDlgItem(IDC_BUTTON_FIND)->EnableWindow(TRUE);
    return 0;
}

void CAdBookDlg::RememberSuccessfulSearchString()
{
    const int itemCount = _searchFilters.GetItemCount();
    for (int i = 0; i < itemCount; ++i)
    {
        const CString s = _searchFilters.GetItemText(i, ValueColumnId);
        if (CB_ERR == _valList.FindStringExact(0, s))
        {
            _valList.InsertString(0, s);
            if (_valList.GetCount() > static_cast<int>(valListMaxItemCount))
            {
                _valList.DeleteString(_valList.GetCount() - 1);
            }
        }
    }
}

LRESULT CAdBookDlg::OnUmStop(WPARAM, LPARAM)
{
    size_t itemCount = 0;
    {
        std::lock_guard<decltype(_peopleMutex)> lg(_peopleMutex);
        itemCount = _people.size();
    }

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
        _adSearcher->Wait();
        UpdateAddButtonState();
        UpdateRemoveButtonState();
        RefreshPersonButtons();
    }
    catch (const adbook::Error & e)
    {
        AfxMessageBox(e.What(), MB_ICONERROR);
    }
    catch (const std::exception & )
    {
        AfxMessageBox(IDS_UNKNOWN_ERROR_DURING_SEARCH, MB_ICONERROR);
    }
    return 0;
}

void CAdBookDlg::DisplayPersonDetails(const int personIdx)
{
    std::unique_lock<decltype(_peopleMutex)> lk(_peopleMutex);
    const auto & person = _people.at(personIdx);
    auto & attributes = adbook::Attributes::GetInstance();
    auto fillOutListWithProperties = [&person,this](CListCtrl & listCtrl)
    {
        int itemCount = listCtrl.GetItemCount();
        for (int i = 0; i < itemCount; ++i)
        {
            const auto attrId = static_cast<adbook::Attributes::AttrId>(listCtrl.GetItemData(i));
            const auto attrName = adbook::Attributes::GetInstance().GetLdapAttrName(attrId);
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
    fillOutListWithProperties(_contactDetails1);
    fillOutListWithProperties(_contactDetails2);
    const adbook::BinaryAttrVal bav = person.GetBinaryAttr(
        attributes.GetLdapAttrName(adbook::Attributes::ThumbnailPhoto)
    );
    lk.unlock();

    ResizeContactDetailsCols();
    RefreshPersonButtons();
    if (!bav.empty())
    {
        _photo.Load(bav);
    }
    else
    {
        _photo.Unload();
        _photo.Invalidate();
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
        personIdx = _searchResults.GetSelectionMark();
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
        attrIdx1 = _contactDetails1.GetSelectionMark();
    }
    if (LB_ERR == attrIdx2)
    {
        attrIdx2 = _contactDetails2.GetSelectionMark();
    }
    auto getAttrId = [](CListCtrl & contactDetails, const int attrIdx) -> adbook::Attributes::AttrId
    {
        if (LB_ERR == attrIdx)
        {
            return adbook::Attributes::AttrIdEnd;
        }
        return static_cast<adbook::Attributes::AttrId>(contactDetails.GetItemData(attrIdx));
    };
    const auto id1 = (attrIdx1 == LB_ERR ? getAttrId(_contactDetails1, attrIdx1) :
                                        static_cast<adbook::Attributes::AttrId>(_contactDetails1.GetItemData(attrIdx1)));
    const auto id2 = (attrIdx2 == LB_ERR ? getAttrId(_contactDetails2, attrIdx2) :
                                        static_cast<adbook::Attributes::AttrId>(_contactDetails2.GetItemData(attrIdx2)));
    const bool searchingIsRunning = _adSearcher->IsStarted();

    copy1->EnableWindow(adbook::Attributes::AttrIdEnd != id1 &&
        !_contactDetails1.GetItemText(attrIdx1, ContactDetailsAttrValueCol).IsEmpty());
    copy2->EnableWindow(adbook::Attributes::AttrIdEnd != id2 &&
        !_contactDetails2.GetItemText(attrIdx2, ContactDetailsAttrValueCol).IsEmpty());

    std::lock_guard<decltype(_peopleMutex)> lg(_peopleMutex);
    const auto & person = _people.at(personIdx);
    edit1->EnableWindow(person.IsAttributeWritable(id1) && !searchingIsRunning);
    edit2->EnableWindow(person.IsAttributeWritable(id2) && !searchingIsRunning);
    selectPhoto->EnableWindow(person.IsAttributeWritable(adbook::Attributes::ThumbnailPhoto));
}

void CAdBookDlg::OnLvnGetdispinfoListResults(NMHDR *pNMHDR, LRESULT *pResult)
{
    NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
    if (pDispInfo->item.mask & LVIF_TEXT)
    {
        std::lock_guard<decltype(_peopleMutex)> lg(_peopleMutex);
        const auto & person = _people.at(pDispInfo->item.iItem);
        const auto id = _searchResultsColIdx.at(pDispInfo->item.iSubItem);
        auto & attributes = adbook::Attributes::GetInstance();
        const auto attrName = attributes.GetLdapAttrName(id);
        if (person.IsAttributeSet(attrName))
        {
            StringCchCopyW(pDispInfo->item.pszText, pDispInfo->item.cchTextMax, person.GetStringAttr(attrName).c_str());
        }
        else {
            StringCchCopyW(pDispInfo->item.pszText, pDispInfo->item.cchTextMax, L"");
        }
    }
    *pResult = 0;
}

void CAdBookDlg::Sort(const int colId, const bool ascending)
{
    CWaitCursor wc;
    const auto id = _searchResultsColIdx.at(colId);
    auto & attributes = adbook::Attributes::GetInstance();
    const auto sortAttr = attributes.GetLdapAttrName(id);
    auto pred = [&sortAttr, ascending](const adbook::AdPersonDesc & itemLeft, const adbook::AdPersonDesc & itemRight)
    {
        const auto left = itemLeft.GetStringAttr(sortAttr), right = itemRight.GetStringAttr(sortAttr);
        return ascending ? left > right : right > left;
    };

    {
        std::lock_guard<decltype(_peopleMutex)> lg(_peopleMutex);
        std::stable_sort(_people.begin(), _people.end(), pred);
    }
    _searchResults.Invalidate();
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
    CHeaderCtrl * headerCtrl = _searchResults.GetHeaderCtrl();

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
    CopySelectedAttrValueToClipboard(_contactDetails1, ContactDetailsAttrValueCol);
}

void CAdBookDlg::OnBnClickedButtonCopy2()
{
    CopySelectedAttrValueToClipboard(_contactDetails2, ContactDetailsAttrValueCol);
}

void CAdBookDlg::ApplyNewValue(adbook::AdPersonDesc & person, const adbook::Attributes::AttrId id, const CString & newValue)
{
    auto & attributes = adbook::Attributes::GetInstance();
    const auto attrName = attributes.GetLdapAttrName(id);
    auto connectionParams = _appSettings.GetConnectionParams();
    auto connector = _adFactory->CreateConnector();
    connector->Connect(connectionParams, person.GetDn());

    if (adbook::Attributes::CommonName == id)   // special handling for CN
    {
        if (newValue.IsEmpty())
        {
            HR_ERROR(E_INVALIDARG);
        }
        connector->Rename(static_cast<PCWSTR>(newValue));
        auto dn = connector->DownloadStringAttr(adbook::AdAttrDn);
        person.SetStringAttr(adbook::AdAttrDn, dn);
    }
    else
    {
        CWaitCursor wc;
        connector->UploadStringAttr(attrName, static_cast<PCWSTR>(newValue));
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
    auto & attributes = adbook::Attributes::GetInstance();
    const auto aid = static_cast<adbook::Attributes::AttrId>(personDetails.GetItemData(selMark));
    if (!attributes.IsAttrSupported(aid))
    {
        return;
    }
    const int selPerson = GetSelectedPersonIdx();
    if (LB_ERR == selPerson)
    {
        return;
    }
    std::unique_lock<decltype(_peopleMutex)> lg(_peopleMutex);
    auto & person = _people.at(static_cast<size_t>(selPerson));
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
        AfxMessageBox(e.What(), MB_ICONERROR);
        return;
    }
    catch (const std::exception &)
    {
        AfxMessageBox(IDS_UNKNOWN_ERROR, MB_ICONERROR);
        return;
    }
    lg.unlock();
    DisplayPersonDetails(selPerson);
    _searchResults.RedrawItems(selPerson, selPerson);
}

void CAdBookDlg::OnBnClickedButtonChange1()
{
    DisplaySvAttrEditor(_contactDetails1);
}

void CAdBookDlg::OnBnClickedButtonChange2()
{
    DisplaySvAttrEditor(_contactDetails2);
}

void CAdBookDlg::OnSelectPhoto()
{
    CString filter;
    VERIFY(filter.LoadString(IDS_SELECT_PHOTO_DLG_FILTER));
    CFileDialog fd(TRUE, nullptr, nullptr, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST, filter, this);
    auto & attributes = adbook::Attributes::GetInstance();
    const auto photoMaxSizeInBytes = attributes.GetAttrMaxLength(adbook::Attributes::ThumbnailPhoto);
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

    try
    {
        std::lock_guard<decltype(_peopleMutex)> lk(_peopleMutex);
        auto & person = _people.at(static_cast<size_t>(selPerson));
        const auto dn = person.GetDn();
        const auto attrName = attributes.GetLdapAttrName(adbook::Attributes::ThumbnailPhoto);
        CString confirm;
        confirm.Format(IDS_PHOTO_UPLOAD_CONFIRM, dn.c_str(), attrName.c_str());
        if (IDOK != AfxMessageBox(confirm, MB_ICONQUESTION | MB_OKCANCEL))
        {
            return;
        }
        auto connector = _adFactory->CreateConnector();
        CWaitCursor wc;
        connector->Connect(_appSettings.GetConnectionParams(), dn);
        connector->UploadBinaryAttr(attrName, photoBuf);
        person.SetBinaryAttr(attrName, photoBuf);
    }
    catch (const adbook::Error & e)
    {
        AfxMessageBox(e.What(), MB_ICONERROR);
    }
    catch (const std::exception &)
    {
        AfxMessageBox(IDS_UNKNOWN_ERROR, MB_ICONERROR);
    }
    DisplayPersonDetails(selPerson);
}

void CAdBookDlg::OnClearPhoto()
{
    const int selPerson = GetSelectedPersonIdx();
    if (LB_ERR == selPerson)
    {
        return;
    }

    try
    {
        std::lock_guard<decltype(_peopleMutex)> lk(_peopleMutex);
        auto & person = _people.at(static_cast<size_t>(selPerson));
        const auto dn = person.GetDn();
        auto connector = _adFactory->CreateConnector();

        CWaitCursor wc;
        connector->Connect(_appSettings.GetConnectionParams(), dn);
        auto & attributes = adbook::Attributes::GetInstance();
        const auto attrName = attributes.GetLdapAttrName(adbook::Attributes::ThumbnailPhoto);
        connector->UploadBinaryAttr(attrName, adbook::BinaryAttrVal());
        person.SetBinaryAttr(attrName, adbook::BinaryAttrVal());
    }
    catch (const adbook::Error & e)
    {
        AfxMessageBox(e.What(), MB_ICONERROR);
    }
    catch (const std::exception &)
    {
        AfxMessageBox(IDS_UNKNOWN_ERROR, MB_ICONERROR);
    }
    DisplayPersonDetails(selPerson);
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
    const int curSel = _valList.GetCurSel();
    if (CB_ERR != curSel)
    {
        CString text;
        _valList.GetLBText(curSel, text);
        text.Trim();
        GetDlgItem(IDC_BUTTON_ADD)->EnableWindow(!text.IsEmpty());
    }
}

void CAdBookDlg::OnNMDblclkContactDetails1(NMHDR * /*pNMHDR*/ , LRESULT *pResult)
{
    if (GetDlgItem(IDC_BUTTON_CHANGE1)->IsWindowEnabled())
    {
        DisplaySvAttrEditor(_contactDetails1);
    }
    *pResult = 0;
}

void CAdBookDlg::OnNMDblclkContactDetails2(NMHDR * /*pNMHDR*/ , LRESULT *pResult)
{
    if (GetDlgItem(IDC_BUTTON_CHANGE2)->IsWindowEnabled())
    {
        DisplaySvAttrEditor(_contactDetails2);
    }
    *pResult = 0;
}
