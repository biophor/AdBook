#include "stdafx.h"
#include "AppSettings.h"

const wchar_t * connectionSettingsSection = L"Connection";
const wchar_t * dcNameParam = L"dc";
const wchar_t * userNameParam = L"userName";
const wchar_t * passwordParam = L"password";
const wchar_t * currentUserCredParam = L"currentUserCred";
const wchar_t * currentDomainParam = L"currentDomain";
const wchar_t * mainWndSection = L"MainWindow";
const wchar_t * leftParam = L"left";
const wchar_t * rightParam = L"right";
const wchar_t * topParam = L"top";
const wchar_t * bottomParam = L"bottom";
const wchar_t * searchFilterColWidthParam = L"SearchFilterColWidth";
const wchar_t * closeToTrayParam = L"CloseToTray";
const wchar_t * minimizeToTrayParam = L"MinimizeToTray";
const wchar_t * displayPasswordParam = L"displayPassword";
const wchar_t * forgetPasswordParam = L"forgetPassword";

AppSettings::AppSettings()
{
    
}

AppSettings::~AppSettings()
{

}

void AppSettings::SaveSettings()
{
    CWinApp * app = AfxGetApp();
    VERIFY(app);
    VERIFY(app->WriteProfileStringW(connectionSettingsSection, dcNameParam, cs_.GetDC()));
    VERIFY(app->WriteProfileStringW(connectionSettingsSection, userNameParam, cs_.GetLogin()));
    if (cs_.ForgetPassword())
    {
        VERIFY(app->WriteProfileStringW(connectionSettingsSection, passwordParam, L""));
    }
    else
    {
        VERIFY(app->WriteProfileStringW(connectionSettingsSection, passwordParam, cs_.GetPassword()));
    }
    
    VERIFY(app->WriteProfileInt(connectionSettingsSection, currentUserCredParam, cs_.CurrentUserCredentials() ? 1 : 0));
    VERIFY(app->WriteProfileInt(connectionSettingsSection, currentDomainParam, cs_.CurrentDomain() ? 1 : 0));
    VERIFY(app->WriteProfileInt(mainWndSection, leftParam, mws_.GetRect().left));
    VERIFY(app->WriteProfileInt(mainWndSection, topParam, mws_.GetRect().top));
    VERIFY(app->WriteProfileInt(mainWndSection, rightParam, mws_.GetRect().right));
    VERIFY(app->WriteProfileInt(mainWndSection, bottomParam, mws_.GetRect().bottom));
    VERIFY(app->WriteProfileInt(mainWndSection, minimizeToTrayParam, mws_.MinimizeToTray()));
    VERIFY(app->WriteProfileInt(mainWndSection, closeToTrayParam, mws_.CloseToTray()));
    VERIFY(app->WriteProfileInt(connectionSettingsSection, displayPasswordParam, cs_.DisplayPassword()));
    VERIFY(app->WriteProfileInt(connectionSettingsSection, forgetPasswordParam, cs_.ForgetPassword()));
    CString s;
    const auto & rv = mws_.GetFilterColWidth();
    std::wstringstream ss;
    std::for_each(rv.begin(), rv.end(), [&ss](int colWidth) { ss << colWidth << L" "; });
    s = ss.str().c_str();
    VERIFY(app->WriteProfileStringW(mainWndSection, searchFilterColWidthParam, s));
}

void AppSettings::LoadSettings()
{
    CWinApp * app = AfxGetApp();
    VERIFY(app);
    cs_.SetDC(static_cast<const wchar_t *>(app->GetProfileString(connectionSettingsSection, dcNameParam)));
    cs_.SetLogin(static_cast<const wchar_t *>(app->GetProfileString(connectionSettingsSection, userNameParam)));
    cs_.SetPassword(static_cast<const wchar_t *>(app->GetProfileString(connectionSettingsSection, passwordParam)));
    cs_.CurrentUserCredentials(!!app->GetProfileInt(connectionSettingsSection, currentUserCredParam, 1));
    cs_.CurrentDomain(!!app->GetProfileInt(connectionSettingsSection, currentDomainParam, 1));
    cs_.DisplayPassword(!!app->GetProfileInt(connectionSettingsSection, displayPasswordParam, 0));
    cs_.ForgetPassword(!!app->GetProfileInt(connectionSettingsSection, forgetPasswordParam, 1));
    CRect rc;
    rc.left = app->GetProfileInt(mainWndSection, leftParam, 0);
    rc.right = app->GetProfileInt(mainWndSection, rightParam, 0);
    rc.top = app->GetProfileInt(mainWndSection, topParam, 0);
    rc.bottom = app->GetProfileInt(mainWndSection, bottomParam, 0);
    mws_.CloseToTray(!!app->GetProfileInt(mainWndSection, closeToTrayParam, 0));
    mws_.MinimizeToTray(!!app->GetProfileInt(mainWndSection, minimizeToTrayParam, 0));
    mws_.SetRect(rc);
    const std::wstring s = static_cast<const wchar_t *>(app->GetProfileString(mainWndSection, searchFilterColWidthParam));    
    std::wstringstream ss(s);
    std::istream_iterator<int, wchar_t> iter(ss);
    std::istream_iterator<int, wchar_t> end;
    std::vector<int> v;
    std::for_each(iter, end, [&v](int colWidth) { v.push_back(colWidth); });
    mws_.SetFilterColWidth(v);
}

