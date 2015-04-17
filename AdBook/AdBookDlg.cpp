
// AdBookDlg.cpp : implementation file
//

#include "stdafx.h"
#include "error.h"
#include "AdBook.h"
#include "AdBookDlg.h"
#include "afxdialogex.h"
#include "SettingsDlg.h"

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

CAdBookDlg::CAdBookDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAdBookDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAdBookDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_ATTR, attrList_);
    DDX_Control(pDX, IDC_COMBO_CONDITION, condList_);
    DDX_Control(pDX, IDC_COMBO_TEXT, valList_);
    DDX_Control(pDX, IDC_LIST_FILTERS, searchFilters_);
    DDX_Control(pDX, IDC_LIST_RESULTS, searchResults_);
}

void CAdBookDlg::InitAttrList()
{
    VERIFY(attrList_.SetItemData(attrList_.AddString(L"Any attribute"), AllAttributes) != CB_ERR);
    VERIFY(attrList_.SetItemData(attrList_.AddString(L"Common Name"), LdapRequest::CommonName) != CB_ERR);
    VERIFY(attrList_.SetItemData(attrList_.AddString(L"Email"), LdapRequest::Email) != CB_ERR);
    VERIFY(attrList_.SetItemData(attrList_.AddString(L"Company"), LdapRequest::Company) != CB_ERR);
    VERIFY(attrList_.SetItemData(attrList_.AddString(L"Title"), LdapRequest::Title) != CB_ERR);
    VERIFY(attrList_.SetItemData(attrList_.AddString(L"Department"), LdapRequest::Department) != CB_ERR);
    VERIFY(attrList_.SetItemData(attrList_.AddString(L"State or province"), LdapRequest::State) != CB_ERR);
    VERIFY(attrList_.SetItemData(attrList_.AddString(L"Employee ID"), LdapRequest::EmpId) != CB_ERR);
    VERIFY(attrList_.SetItemData(attrList_.AddString(L"Locality"), LdapRequest::Locality) != CB_ERR);
    attrList_.SetCurSel(0);
}

void CAdBookDlg::InitConditionList()
{
    VERIFY(condList_.SetItemData(condList_.AddString(L"Contains"), LdapRequest::Contains) != CB_ERR);
    VERIFY(condList_.SetItemData(condList_.AddString(L"Begin with"), LdapRequest::BeginWith) != CB_ERR);
    VERIFY(condList_.SetItemData(condList_.AddString(L"Exact match"), LdapRequest::ExactMatch) != CB_ERR);
    VERIFY(condList_.SetItemData(condList_.AddString(L"End with"), LdapRequest::EndWith) != CB_ERR);
    condList_.SetCurSel(0);
}

void CAdBookDlg::InitSearchResultList()
{
    searchResults_.SetExtendedStyle(searchResults_.GetExtendedStyle() | LVS_EX_FULLROWSELECT);
    
    const int numCols = attrList_.GetCount();
    for (int i = 0, colIdx=0; i < numCols; ++i)
    {
        CString title;
        attrList_.GetLBText(i, title);
        const LdapRequest::AttrId id = static_cast<LdapRequest::AttrId>(attrList_.GetItemData(i));
        if (LdapRequest::attrInfo.find(id) == LdapRequest::attrInfo.end())
        {
            continue;
        }
        searchResults_.InsertColumn(colIdx, title, LVCFMT_LEFT, 100);
        searchResultsColIdx_[colIdx] = id;
        ++colIdx;
    }
    
    searchResults_.SetColumnWidth(searchResults_.GetHeaderCtrl()->GetItemCount()-1, LVSCW_AUTOSIZE_USEHEADER);
    
    /*
    auto fcw = theApp.GetAppSettings().GetMainWndSettings().GetFilterColWidth();
    const int colCount = searchResults_.GetHeaderCtrl()->GetItemCount();
    if (fcw.size() == colCount)
    {
        for (int i = 0; i < colCount; ++i)
        {
            if (fcw[i])
            {
                searchResults_.SetColumnWidth(i, fcw[i]);
            }
        }
    }
    */
}

void CAdBookDlg::InitMinSize()
{
    CRect rc;
    GetWindowRect(&rc);
    minSize_.cx = rc.right - rc.left;
    minSize_.cy = rc.bottom - rc.top;
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
    
    auto fcw = theApp.GetAppSettings().GetMainWndSettings().GetFilterColWidth();
    const int colCount = searchFilters_.GetHeaderCtrl()->GetItemCount();
    if (fcw.size() == colCount)
    {
        for (int i = 0; i < colCount; ++i)
        {
            if (fcw[i])
            {
                searchFilters_.SetColumnWidth(i, fcw[i]);
            }
        }
    }
}

void CAdBookDlg::SaveSearchFilterCols()
{
    std::vector<int> fcw;
    int colCount = searchFilters_.GetHeaderCtrl()->GetItemCount();
    for (int i = 0; i < colCount; ++i)
    {
        const int colWidth = searchFilters_.GetColumnWidth(i);
        fcw.push_back(colWidth);
    }
    theApp.GetAppSettings().GetMainWndSettings().SetFilterColWidth(fcw);
}

void CAdBookDlg::SaveWindowPosition()
{
    CRect rect;
    GetWindowRect(&rect);
    mws_.SetRect(rect);
}

void CAdBookDlg::RestoreWindowPosition()
{                 
    const CRect rect = mws_.GetRect();
    if (rect.Width() && rect.Height())
    {
        MoveWindow(rect.left, rect.top, rect.Width(), rect.Height());
    }    
}
                  
BEGIN_MESSAGE_MAP(CAdBookDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
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
ON_NOTIFY(LVN_ITEMACTIVATE, IDC_LIST_FILTERS, &CAdBookDlg::OnLvnItemActivateListFilters)
ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_FILTERS, &CAdBookDlg::OnLvnItemchangedListFilters)
ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_RESULTS, &CAdBookDlg::OnLvnGetdispinfoListResults)
ON_MESSAGE(UM_REFRESH_TOTAL, &CAdBookDlg::OnUmRefreshTotal)
ON_MESSAGE(UM_START, &CAdBookDlg::OnUmStart)
ON_MESSAGE(UM_STOP, &CAdBookDlg::OnUmStop)
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
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
    valList_.LimitText(200);

    adSearcher_.SetCallbacks(
        std::bind(&CAdBookDlg::OnNewItem, this, std::placeholders::_1), 
        std::bind(&CAdBookDlg::OnStart, this),
        std::bind(&CAdBookDlg::OnStop, this));

    CheckDlgButton(IDC_RADIO_ALL, BST_CHECKED);
    InitMinSize();
    InitAttrList();
    InitConditionList();
    InitSearchFiltersList();
    InitSearchResultList();
    UpdateAddButtonState();
    UpdateRemoveButtonState();

    wndAnchor_.Init(GetSafeHwnd());
    wndAnchor_.Add(IDOK, WindowAnchor::BottomRight);
    wndAnchor_.Add(IDCANCEL, WindowAnchor::BottomRight);
    wndAnchor_.Add(IDC_BUTTON_ADD, WindowAnchor::TopRight);
    wndAnchor_.Add(IDC_BUTTON_FIND, WindowAnchor::TopRight);
    wndAnchor_.Add(IDC_BUTTON_REMOVE, WindowAnchor::TopRight);
    wndAnchor_.Add(IDC_BUTTON_SETTINGS, WindowAnchor::BottomLeft);
    wndAnchor_.Add(IDC_COMBO_TEXT, WindowAnchor::TopLeft | WindowAnchor::Right);
    wndAnchor_.Add(IDC_LIST_FILTERS, WindowAnchor::TopLeft | WindowAnchor::BottomRight, 1.0f, 1.0f, 1.0f, 0.2f);
    wndAnchor_.Add(IDC_CONTACT_DETAILS_CAPTION, WindowAnchor::BottomLeft);
    wndAnchor_.Add(IDC_CONTACT_DETAILS, WindowAnchor::Left | WindowAnchor::BottomRight);
    wndAnchor_.Add(IDC_LIST_RESULTS, WindowAnchor::TopLeft | WindowAnchor::BottomRight, 1.0f, 1.0f, 0.8f, 1.0f);
    wndAnchor_.Add(IDC_RADIO_ONE, WindowAnchor::TopLeft | WindowAnchor::Bottom, 1.0f, 1.0f, 0.8f, 0.2f);
    wndAnchor_.Add(IDC_RADIO_ALL, WindowAnchor::TopLeft | WindowAnchor::Bottom, 1.0f, 1.0f, 0.8f, 0.2f);
    wndAnchor_.Add(IDC_RESULT_CAPTION, WindowAnchor::TopLeft | WindowAnchor::BottomRight, 1.0f, 1.0f, 0.8f, 0.2f);
    wndAnchor_.Add(IDC_STATIC_TOTAL, WindowAnchor::Top | WindowAnchor::BottomRight, 1.0f, 1.0f, 0.8f, 0.2f);
    RestoreWindowPosition();
	return TRUE;  // return TRUE  unless you set the focus to a control
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

// If you add a minimize button to your dialog, you will need the code below
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
}

void CAdBookDlg::OnBnClickedButtonAdd()
{
    int curSel = attrList_.GetCurSel();
    VERIFY(LB_ERR != curSel);
    const LdapRequest::AttrId attrId = static_cast<LdapRequest::AttrId>(attrList_.GetItemData(curSel));
    CString attrDisplayName;
    GetDlgItemText(IDC_COMBO_ATTR, attrDisplayName);

    curSel = condList_.GetCurSel();
    VERIFY(LB_ERR != curSel);
    const LdapRequest::MathingRule rule = static_cast<LdapRequest::MathingRule>(condList_.GetItemData(curSel));
    CString conditionDisplayName;
    GetDlgItemText(IDC_COMBO_CONDITION, conditionDisplayName);

    CString val;
    GetDlgItemText(IDC_COMBO_TEXT, val);
    VERIFY(!val.IsEmpty());
    
    LVFINDINFO fi = { LVFI_STRING, static_cast<LPCWSTR>(attrDisplayName) };
    int idx = searchFilters_.FindItem(&fi);
    if (idx == -1)
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
    VERIFY(selCount == 1);
    const int selMark = searchFilters_.GetSelectionMark();
    VERIFY(selMark != -1);
    VERIFY(searchFilters_.DeleteItem(selMark));
}


void CAdBookDlg::OnBnClickedButtonFind()
{    
    try
    {
        CWaitCursor wc;
        if (adSearcher_.IsStarted())
        {
            adSearcher_.Stop();
            adSearcher_.Wait();
        }
        else
        {
            adSearcher_.Start(ConstructLdapRequest());
        }
    }
    catch (const Error & e)
    {
        CString error;
        VERIFY(error.LoadString(IDS_FAILED_TO_CONNECT));
        error += L" ";
        error += e.What();
        AfxMessageBox(error, MB_ICONERROR);
    }
    catch (const std::exception &)
    {
        AfxMessageBox(IDS_FAILED_TO_CONNECT, MB_ICONERROR);
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
    SaveWindowPosition();
    SaveSearchFilterCols();

    if (adSearcher_.IsStarted())
    {
        adSearcher_.Stop();
        adSearcher_.Wait();
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
    if (adSearcher_.IsStarted())
    {
        adSearcher_.Stop();
        adSearcher_.Wait();
    }

    CDialogEx::OnClose();
}

void CAdBookDlg::UpdateAddButtonState()
{
    CString s;
    GetDlgItemText(IDC_COMBO_TEXT, s);
    s.Trim();
    GetDlgItem(IDC_BUTTON_ADD)->EnableWindow(!s.IsEmpty());
}

void CAdBookDlg::UpdateRemoveButtonState()
{
    const UINT selCount = searchFilters_.GetSelectedCount();    
    GetDlgItem(IDC_BUTTON_REMOVE)->EnableWindow(selCount != 0);
}

void CAdBookDlg::OnCbnEditchangeComboText()
{
    UpdateAddButtonState();
}


void CAdBookDlg::OnCbnSelchangeComboText()
{
    // TODO: Add your control notification handler code here
}


void CAdBookDlg::OnLvnItemActivateListFilters(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    // TODO: Add your control notification handler code here
    
    *pResult = 0;
}


void CAdBookDlg::OnLvnItemchangedListFilters(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    // TODO: Add your control notification handler code here
    UpdateRemoveButtonState();
    *pResult = 0;
}

LdapRequest CAdBookDlg::ConstructLdapRequest()
{
    LdapRequest lr;    
    const int itemCount = searchFilters_.GetItemCount();
    for (int i = 0; i < itemCount; ++i)
    {
        const DWORD itemData = static_cast<DWORD>(searchFilters_.GetItemData(i));
        const LdapRequest::AttrId id = static_cast<LdapRequest::AttrId>(LOWORD(itemData));
        const LdapRequest::MathingRule mr = static_cast<LdapRequest::MathingRule>(HIWORD(itemData));
        const CString val = searchFilters_.GetItemText(i, ValueColumnId);
        if (id == AllAttributes)
        {
            for (const auto & ii : LdapRequest::attrInfo)
            {
                lr.AddRule(ii.first, mr, val);
                lr.AddOR();
            }
        }
        else
        {
            lr.AddRule(id, mr, val);
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
    lr.AddRule(L"objectCategory", LdapRequest::ExactMatch, L"person");
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
    s.LoadString(IDS_TOTAL);
    s.AppendFormat(L" %d", itemCount);
    SetDlgItemText(IDC_STATIC_TOTAL, s);    
    return 0;
}

void CAdBookDlg::OnNewItem(std::unique_ptr<AdSearcher::Item> itemPtr)
{    
    static time_t prevTime = time(NULL);
    time_t curTime = time(NULL);
    int itemCount = 0;
    {
        std::lock_guard<decltype(searchResultsMutex_)> lg(searchResultsMutex_);
        contacts_.push_back(*itemPtr);        
    }
    if (curTime - prevTime > 1)
    {
        itemCount = static_cast<int>(contacts_.size());        
        PostMessage(UM_REFRESH_TOTAL, itemCount);        
        prevTime = curTime;
    }    
}

void CAdBookDlg::OnStart()
{
    std::lock_guard<decltype(searchResultsMutex_)> lg(searchResultsMutex_);
    contacts_.clear();
    PostMessage(UM_START);
    PostMessage(UM_REFRESH_TOTAL, 0);
}

void CAdBookDlg::OnStop()
{
    PostMessage(UM_STOP);
    
    int itemCount = 0;
    std::lock_guard<decltype(searchResultsMutex_)> lg(searchResultsMutex_);
    itemCount = static_cast<int>(contacts_.size());
    PostMessage(UM_REFRESH_TOTAL, itemCount);
}

LRESULT CAdBookDlg::OnUmStart(WPARAM, LPARAM)
{
    CString s;
    VERIFY(s.LoadString(IDS_STOP));    
    SetDlgItemText(IDC_BUTTON_FIND, s);
    return 0;
}

LRESULT CAdBookDlg::OnUmStop(WPARAM, LPARAM)
{
    CString s;
    VERIFY(s.LoadString(IDS_FIND));
    SetDlgItemText(IDC_BUTTON_FIND, s);
    return 0;
}

void CAdBookDlg::OnLvnGetdispinfoListResults(NMHDR *pNMHDR, LRESULT *pResult)
{
    std::lock_guard<decltype(searchResultsMutex_)> lg(searchResultsMutex_);
    NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
    if (pDispInfo->item.mask & LVIF_TEXT)
    {
        AdSearcher::Item & contact = contacts_.at(pDispInfo->item.iItem);
        const LdapRequest::AttrId id = searchResultsColIdx_.at(pDispInfo->item.iSubItem);
        LPCWSTR attrName = LdapRequest::attrInfo.at(id);
        auto iter = contact.stringAttrs.find(attrName);
        if (iter != contact.stringAttrs.end())
        {
            auto & l = iter->second;
            CString text;
            for (const auto & i : l)
            {
                text += i;
            }
            wcscpy_s(pDispInfo->item.pszText, pDispInfo->item.cchTextMax, text);
        }
    }    
    *pResult = 0;
}
