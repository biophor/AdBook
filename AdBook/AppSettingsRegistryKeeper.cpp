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

#include "stdafx.h"
#include "shared.h"
#include "AppSettingsRegistryKeeper.h"


namespace
{
    const wchar_t baseSectionName[] = L"base";
    const wchar_t sfBaseSectionName[] = L"sfl";
    const wchar_t numItemsName[] = L"count";
    const wchar_t attrIdName[] = L"attrId";
    const wchar_t ruleIdName[] = L"ruleId";
    const wchar_t attrValName[] = L"attrVal";

    const wchar_t * mainWndSection = L"MainWindow";
    const wchar_t * leftParam = L"left";
    const wchar_t * rightParam = L"right";
    const wchar_t * topParam = L"top";
    const wchar_t * bottomParam = L"bottom";
    const wchar_t * searchFilterColWidthParam = L"SearchFilterColWidth";
    const wchar_t * allConditionsShouldBeMetParam = L"allConditionsShouldBeMet";
    const wchar_t * searchResultColWidthParam = L"SearchResultColWidth";
    const wchar_t * searchResultColOrderParam = L"SearchResultColOrder";
    const wchar_t * searchFilterStringsParam = L"SearchFilterStrings";

    const wchar_t changeSvAttrDlgSection[] = L"ChangeSvAttrDlg";

    const wchar_t connectionSettingsSection[] = L"Connection";
    const wchar_t dcNameParam[] = L"dc";
    const wchar_t userNameParam[] = L"userName";
    const wchar_t protectedPasswordParam[] = L"protectedPassword";
    const wchar_t currentUserCredParam[] = L"currentUserCred";
    const wchar_t currentDomainParam[] = L"currentDomain";
}

void AppSettingsRegistryKeeper::Load(AppSettings & appSettings, bool & passwordReadingFailed)
{
    CWinApp * app = AfxGetApp();
    VERIFY(app);
    auto getint = [app](const wchar_t* section, const wchar_t * param, int def) {
        return (app->GetProfileInt(section, param, def));
    };
    LoadMainWindowsSettings(appSettings.GetMainWndSettings());
    LoadConnectionParams(appSettings.GetConnectionParams(), passwordReadingFailed);

    // change attribute dialog settings

    CRect svAttrEditorPosition =
        CRect(getint(changeSvAttrDlgSection, leftParam, 0), getint(changeSvAttrDlgSection, topParam, 0),
            getint(changeSvAttrDlgSection, rightParam, 0), getint(changeSvAttrDlgSection, bottomParam, 0));
    const bool positionCanBeSet = !(svAttrEditorPosition.IsRectEmpty() || svAttrEditorPosition.IsRectNull());
    if (positionCanBeSet) {
        appSettings.SetSvAttrEditorPosition(svAttrEditorPosition);
    }
}

void AppSettingsRegistryKeeper::Save(const AppSettings & appSettings)
{
    CWinApp * app = AfxGetApp();
    // connection settings
    VERIFY(app);
    CRect svAttrEditorPosition = appSettings.GetSvAttrEditorPosition();
    // change attribute dialog settings
    VERIFY(app->WriteProfileInt(changeSvAttrDlgSection, leftParam, svAttrEditorPosition.left));
    VERIFY(app->WriteProfileInt(changeSvAttrDlgSection, topParam, svAttrEditorPosition.top));
    VERIFY(app->WriteProfileInt(changeSvAttrDlgSection, rightParam, svAttrEditorPosition.right));
    VERIFY(app->WriteProfileInt(changeSvAttrDlgSection, bottomParam, svAttrEditorPosition.bottom));

    SaveMainWindowSettings(appSettings.GetMainWndSettings());
    SaveConnectionParams(appSettings.GetConnectionParams());
}

void AppSettingsRegistryKeeper::LoadMainWindowsSettings(MainWndSettings & mainWndSettings)
{
    CWinApp * app = AfxGetApp();
    auto & mws = mainWndSettings;
    VERIFY(app);
    auto getws = [app](const wchar_t* section, const wchar_t * param) -> std::wstring {
        return static_cast<const wchar_t *>(app->GetProfileString(section, param));
    };
    auto getint = [app](const wchar_t* section, const wchar_t * param, int def) {
        return (app->GetProfileInt(section, param, def));
    };

    // main windows settings
    mws.SetRect(CRect(getint(mainWndSection, leftParam, 0), getint(mainWndSection, topParam, 0),
        getint(mainWndSection, rightParam, 0), getint(mainWndSection, bottomParam, 0)));

    mws.SetCondCombineOp(getint(mainWndSection, allConditionsShouldBeMetParam, 1) ?
        MainWndSettings::ConditionsCombineOperation::And :
        MainWndSettings::ConditionsCombineOperation::Or);

    // string "23 32 34 193" -> vector {23,32,34,193}
    auto s2v = [](const std::wstring & s) {
        IntVector v;
        std::wstringstream ss(s);
        std::istream_iterator<int, wchar_t> iter(ss);
        std::istream_iterator<int, wchar_t> end;
        std::for_each(iter, end, [&v](int d) { v.push_back(d); });
        return v;
    };
    mws.SetFilterColWidth(s2v(getws(mainWndSection, searchFilterColWidthParam)));
    mws.SetResultColWidth(s2v(getws(mainWndSection, searchResultColWidthParam)));
    mws.SetResultColOrder(s2v(getws(mainWndSection, searchResultColOrderParam)));

    auto s2ls = [](const std::wstring & s) {
        MainWndSettings::SearchFilterStrings v;
        std::wstringstream ss(s);
        std::wstring w;
        while (std::getline(ss, w, L'\n'))
        {
            v.push_back(w.c_str());
        }
        return v;
    };
    mws.SetSearchFilterStrings(s2ls(getws(mainWndSection, searchFilterStringsParam)));

    // search filters
    std::list<SearchFilter> seachFilters;
    LoadSearchFilters(seachFilters);
    mws.SetSearchFilters(seachFilters);
}

void AppSettingsRegistryKeeper::SaveMainWindowSettings(const MainWndSettings & mainWndSettings)
{
    CWinApp * app = AfxGetApp();
    auto & mws = mainWndSettings;
    //main windows settings
    VERIFY(app->WriteProfileInt(mainWndSection, leftParam, mws.GetRect().left));
    VERIFY(app->WriteProfileInt(mainWndSection, topParam, mws.GetRect().top));
    VERIFY(app->WriteProfileInt(mainWndSection, rightParam, mws.GetRect().right));
    VERIFY(app->WriteProfileInt(mainWndSection, bottomParam, mws.GetRect().bottom));
    VERIFY(app->WriteProfileInt(mainWndSection, allConditionsShouldBeMetParam,
        mws.GetCondCombineOp() == MainWndSettings::ConditionsCombineOperation::And));

    // vector {23,32,34,193} -> string "23 32 34 193"
    auto v2s = [](const auto & v) {
        CString s;
        std::wstringstream ss;
        std::for_each(v.begin(), v.end(), [&ss](int d) {ss << d << L" "; });
        s = ss.str().c_str();
        return s;
    };
    VERIFY(app->WriteProfileStringW(mainWndSection, searchFilterColWidthParam, v2s(mws.GetFilterColWidth())));
    VERIFY(app->WriteProfileStringW(mainWndSection, searchResultColWidthParam, v2s(mws.GetResultColWidth())));
    VERIFY(app->WriteProfileStringW(mainWndSection, searchResultColOrderParam, v2s(mws.GetResultColOrder())));

    auto ls2s = [](const auto & ls) {
        CString s;
        std::wstringstream ss;
        std::for_each(ls.begin(), ls.end(),
            [&ss](const CString & s) {ss << static_cast<const wchar_t *>(s) << L"\n"; }
        );
        s = ss.str().c_str();
        return s;
    };
    VERIFY(app->WriteProfileStringW(mainWndSection, searchFilterStringsParam, ls2s(mws.GetSearchFilterStrings())));
    // search filters
    SaveSearchFilters(mws.GetSearchFilters());
}

void AppSettingsRegistryKeeper::LoadSearchFilters(std::list<SearchFilter> & searchFilters)
{
    searchFilters.clear();
    auto app = AfxGetApp();
    UINT numItems = app->GetProfileIntW(baseSectionName, numItemsName, 0);
    for (UINT i = 0; i < numItems; ++i)
    {
        SearchFilter sf;
        CString sectionName = CString(sfBaseSectionName) + std::to_wstring(i).c_str();
        sf.attrId = app->GetProfileIntW(sectionName, attrIdName, 0);
        sf.rule = boost::numeric_cast<adbook::LdapRequestBuilder::MatchingRule>(
            app->GetProfileIntW(sectionName, ruleIdName, adbook::LdapRequestBuilder::InvalidMatchingRule)
            );
        sf.attrValue = app->GetProfileStringW(sectionName, attrValName);
        searchFilters.push_back(std::move(sf));
    }
}

void AppSettingsRegistryKeeper::SaveSearchFilters(const std::list<SearchFilter> & searchFilters)
{
    auto app = AfxGetApp();
    const size_t numItems = searchFilters.size();
    VERIFY(app->WriteProfileInt(baseSectionName, numItemsName, boost::numeric_cast<int>(numItems)));
    UINT searchFilterIndex = 0;
    for (const auto & searchFilter : searchFilters)
    {
        CString sectionName = CString(sfBaseSectionName) + std::to_wstring(searchFilterIndex).c_str();
        VERIFY(app->WriteProfileInt(sectionName, attrIdName, searchFilter.attrId));
        VERIFY(app->WriteProfileInt(sectionName, ruleIdName, searchFilter.rule));
        VERIFY(app->WriteProfileStringW(sectionName, attrValName, searchFilter.attrValue));
        ++searchFilterIndex;
    }
}

namespace
{
std::vector<BYTE> ProtectPassword(const std::wstring & password)
{
    if (password.empty())
    {
        return std::vector<BYTE>();
    }
    std::vector<wchar_t> buffToProtect(password.cbegin(), password.cend());
    DATA_BLOB db;
    db.cbData = boost::numeric_cast<DWORD>(password.length() * sizeof(wchar_t));
    db.pbData = reinterpret_cast<BYTE*>(&buffToProtect[0]);
    DATA_BLOB dbr;
    // the following function may fail for several reasons.
    if (!CryptProtectData(&db, nullptr, nullptr, nullptr, nullptr, 0, &dbr))
    {
        HR_ERROR(HRESULT_FROM_WIN32(GetLastError()));
    }
    std::vector<BYTE> result(dbr.pbData, dbr.pbData + boost::numeric_cast<size_t>(dbr.cbData));
    LocalFree(dbr.pbData);
    return result;
}

bool TryToProtectPassword(
    const std::wstring & password,
    std::vector<BYTE> & protectedPassword)
{
    try
    {
        protectedPassword = ProtectPassword(password);
    }
    catch (std::exception &)
    {
        return false;
    }
    return true;
}

std::wstring UnprotectPassword(std::vector<BYTE> protectedPassword)
{
    if (protectedPassword.empty())
    {
        return std::wstring();
    }
    DATA_BLOB db;
    db.cbData = boost::numeric_cast<DWORD>(protectedPassword.size());
    db.pbData = &protectedPassword[0];
    DATA_BLOB dbr;
    if (!CryptUnprotectData(&db, nullptr, nullptr, nullptr, nullptr, 0, &dbr))
    {
        HR_ERROR(HRESULT_FROM_WIN32(GetLastError()));
    }
    return std::wstring(
        reinterpret_cast<const wchar_t *>(dbr.pbData),
        boost::numeric_cast<size_t>(dbr.cbData) / sizeof(wchar_t)
    );
}

bool TryToUnprotectPassword(
    const std::vector<BYTE> & protectedPassword,
    std::wstring & password
)
{
    try
    {
        password = UnprotectPassword(protectedPassword);
    }
    catch (std::exception &)
    {
        return false;
    }
    return true;
}

}

void AppSettingsRegistryKeeper::LoadConnectionParams(
    adbook::ConnectionParams & connectionParams,
    bool & passwordReadingFailed
)
{
    passwordReadingFailed = false;
    CWinApp * app = AfxGetApp();
    VERIFY(app);
    auto getcs = [app](const wchar_t* section, const wchar_t * param) -> CString {
        return app->GetProfileString(section, param);
    };
    auto getbool = [app](const wchar_t* section, const wchar_t * param, bool def) {
        return (!!app->GetProfileInt(section, param, static_cast<int>(def)));
    };
    auto getbin = [app](const wchar_t* section, const wchar_t * param) {
        BYTE * dataPtr = nullptr;
        UINT dataSizeInBytes = 0;
        if (app->GetProfileBinary(section, param, &dataPtr, &dataSizeInBytes) && dataPtr && dataSizeInBytes) {
            auto ret = std::vector<BYTE>(dataPtr, dataPtr + boost::numeric_cast<size_t>(dataSizeInBytes));
            delete[] dataPtr;
            dataPtr = nullptr;
            return ret;
        }
        else {
            return std::vector<BYTE>();
        }
    };
    // connection settings
    connectionParams.SetDomainController(getcs(connectionSettingsSection, dcNameParam));
    connectionParams.SetLogin(getcs(connectionSettingsSection, userNameParam));
    connectionParams.UseCurrentUserCredentials(getbool(connectionSettingsSection, currentUserCredParam, true));
    connectionParams.ConnectDomainYouAreLoggedIn(getbool(connectionSettingsSection, currentDomainParam, true));

    std::vector<BYTE> protectedPassword = getbin(connectionSettingsSection, protectedPasswordParam);
    std::wstring password;
    if (!protectedPassword.empty())
    {
        if (TryToUnprotectPassword(protectedPassword, password))
        {
            connectionParams.SetPassword(password);
        }
        else
        {
            passwordReadingFailed = true;
        }
    }
    else
    {
        connectionParams.SetPassword(L"");
    }
}

void AppSettingsRegistryKeeper::SaveConnectionParams(const adbook::ConnectionParams & connectionParams)
{
    CWinApp * app = AfxGetApp();

    // connection settings
    auto & cs = connectionParams;
    VERIFY(app);
    VERIFY(app->WriteProfileStringW(connectionSettingsSection, dcNameParam, cs.GetDomainController().c_str()));
    VERIFY(app->WriteProfileStringW(connectionSettingsSection, userNameParam, cs.GetLogin().c_str()));
    VERIFY(app->WriteProfileInt(connectionSettingsSection, currentUserCredParam, cs.IsCurrentUserCredentialsUsed() ? 1 : 0));
    VERIFY(app->WriteProfileInt(connectionSettingsSection, currentDomainParam, cs.CurrentDomain() ? 1 : 0));

    std::wstring password = cs.GetPassword();
    if (!password.empty())
    {
        std::vector<BYTE> protectedPassword;
        if (TryToProtectPassword(password, protectedPassword))
        {
            VERIFY(app->WriteProfileBinary(connectionSettingsSection, protectedPasswordParam,
                &protectedPassword[0], boost::numeric_cast<UINT>(protectedPassword.size()))
            );
        }
        else
        {
            VERIFY(app->WriteProfileBinary(connectionSettingsSection, protectedPasswordParam, nullptr, 0));
        }
    }
    else
    {
        VERIFY(app->WriteProfileBinary(connectionSettingsSection, protectedPasswordParam, nullptr, 0));
    }
}

