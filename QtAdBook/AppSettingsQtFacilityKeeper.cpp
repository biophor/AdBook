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

#include "stdafx.h"
#include "QSettingsAutoEndGroup.h"
#include "AppSettingsQtFacilityKeeper.h"

namespace ConnectionSettingsStrings
{
const char connectionSettingsSection[] = "Connection";
const char dcNameParam[] = "dc";
const char userNameParam[] = "userName";
const char passwordParam[] = "password";
const char currentUserCredParam[] = "currentUserCred";
const char currentDomainParam[] = "currentDomain";

}

QString ProtectPassword(const QString & s)
{
    if (s.isEmpty()) {
        return QString();
    }
    auto w = s.toStdWString();
    std::vector<wchar_t> bufToProtect(w.cbegin(), w.cend());
    DATA_BLOB db;
    db.cbData = static_cast<DWORD>(bufToProtect.size() * sizeof(wchar_t));
    db.pbData = (BYTE*)(&bufToProtect[0]);
    DATA_BLOB rdb;
    if (!CryptProtectData(&db, nullptr, nullptr, nullptr, nullptr, 0, &rdb)) {
        throw PasswordEncryptionError(HRESULT_FROM_WIN32(GetLastError()),
            L"CryptProtectData() failed.", __FUNCTIONW__
        );
    }
    QByteArray rba((const char*)rdb.pbData, rdb.cbData);
    LocalFree(rdb.pbData);
    return rba.toBase64();
}

QString UnprotectPassword(const QString & s)
{
    if (s.isEmpty()) {
        return QString();
    }
    QByteArray ba64 = QByteArray::fromStdString(s.toStdString());
    QByteArray ba = QByteArray::fromBase64(ba64);
    DATA_BLOB db;
    db.cbData = static_cast<DWORD>(ba.size());
    db.pbData = (BYTE*)(ba.data());
    DATA_BLOB rdb;
    if (!CryptUnprotectData(&db, nullptr, nullptr, nullptr, nullptr, 0, &rdb)) {
        throw PasswordDecryptionError(HRESULT_FROM_WIN32(GetLastError()),
            L"CryptUnprotectData() failed.", __FUNCTIONW__
        );
    }
    QString ret = QString::fromWCharArray((const wchar_t*)rdb.pbData, rdb.cbData / sizeof(wchar_t));
    LocalFree(rdb.pbData);
    return ret;
}


void AppSettingsQtFacilityKeeper::Load(AppSettings & appSettings)
{
    adbook::ConnectionParams connectionParams;
    LoadConnectionParams(connectionParams);
    appSettings.SetConnectionParams(connectionParams);

    QRect rc;
    LoadSettingsDlgGeometry(rc);
    appSettings.SetSettingsDlgGeometry(rc);

    MainWndSettings mws;
    LoadMainWndSettings(mws);
    appSettings.SetMainWndSettings(mws);

    FilterListSettings fls;
    LoadFilterListSettings(fls);
    appSettings.SetFilterListSettings(fls);
}

void AppSettingsQtFacilityKeeper::Save(const AppSettings & appSettings)
{
    SaveConnectionParams(appSettings.GetConnectionParams());
    SaveSettingsDlgGeometry(appSettings.GetSettingsDlgGeometry());
    SaveMainWndSettings(appSettings.GetMainWndSettings());
    SaveFilterListSettings(appSettings.GetFilterListSettings());
}

void AppSettingsQtFacilityKeeper::LoadConnectionParams(adbook::ConnectionParams & connectionParams)
{
    using namespace ConnectionSettingsStrings;
    QSettings settings(QApplication::organizationName(), QApplication::applicationName());

    QSettingsAutoGroup autoGroup(settings, connectionSettingsSection);
    connectionParams.SetAddress(settings.value(dcNameParam).toString().toStdWString());
    connectionParams.SetLogin(settings.value(userNameParam).toString().toStdWString());
    QString protectedPassword = settings.value(passwordParam).toString();
    connectionParams.Set_ConnectAsCurrentUser(settings.value(currentUserCredParam, true).toBool());
    connectionParams.Set_ConnectDomainYouLoggedIn(settings.value(currentDomainParam, true).toBool());
    // ProtectPassword() can throw an exception in rare cases
    QString unprotectedPassword = UnprotectPassword(protectedPassword);
    connectionParams.SetPassword(unprotectedPassword.toStdWString());
}

void AppSettingsQtFacilityKeeper::SaveConnectionParams(const adbook::ConnectionParams & connectionParams)
{
    using namespace ConnectionSettingsStrings;
    QSettings settings(QApplication::organizationName(), QApplication::applicationName());
    QSettingsAutoGroup autoGroup(settings, connectionSettingsSection);
    settings.setValue(dcNameParam, QString::fromStdWString(connectionParams.GetAddress()));
    settings.setValue(userNameParam, QString::fromStdWString(connectionParams.GetLogin()));
    settings.setValue(currentUserCredParam, connectionParams.Get_ConnectAsCurrentUser());
    settings.setValue(currentDomainParam, connectionParams.Get_ConnectDomainYouLoggedIn());
    // ProtectPassword() can throw an exception in very rare cases
    QString protectedPassword = ProtectPassword(QString::fromStdWString(connectionParams.GetPassword()));
    settings.setValue(passwordParam, protectedPassword);
}

namespace SettingsWndSettingsStrings
{
const QString wndPos = "SettingsWindowPosition";
}

void AppSettingsQtFacilityKeeper::LoadSettingsDlgGeometry(QRect & geom)
{
    QSettings settings(QApplication::organizationName(), QApplication::applicationName());
    if (settings.contains(SettingsWndSettingsStrings::wndPos)) {
        geom = settings.value(SettingsWndSettingsStrings::wndPos, QRect()).toRect();
    }
}

void AppSettingsQtFacilityKeeper::SaveSettingsDlgGeometry(const QRect & geom)
{
    QSettings settings(QApplication::organizationName(), QApplication::applicationName());
    settings.setValue(SettingsWndSettingsStrings::wndPos, geom);
}

namespace MainWndSettingsStrings
{
    const QString sectionName = "MainWindow";
    const QString wndPosParam = "MainWindowPosition";
    const QString contactSplitterParam = "ContactSplitter";
    const QString mainSplitterParam = "MainSplitter";
    const QString prevSearchAttrValuesParam = "PrevSearchAttrValues";
    const QString wndMaximizedParam = "maximixed";
    const QString filterTopLevelRuleParam = "AllTheConditionsShouldBeMet";
    const QString contactsHeaderStateParam = "ContactsHeaderState";
    const QString filtersHeaderStateParam = "FiltersHeaderState";
    const QString propertiesHeaderStateParam = "PropertiesHeaderState";
}

void AppSettingsQtFacilityKeeper::LoadMainWndSettings(MainWndSettings & mws)
{
    auto VarListToIntList = [](QVariantList vl) {
        QList<int> r; for (auto v : vl) r.push_back(v.toInt()); return r;
    };
    using namespace MainWndSettingsStrings;

    QSettings settings(QApplication::organizationName(), QApplication::applicationName());
    QSettingsAutoGroup autoGroupExternal(settings, sectionName);

    mws.SetFlagAllConditionsShouldBeMet(settings.value(filterTopLevelRuleParam, true).toBool());
    mws.SetFlagMaximized(settings.value(wndMaximizedParam, false).toBool());
    mws.SetGeometry(settings.value(wndPosParam, QRect()).toRect());

    QVariantList csvl = settings.value(contactSplitterParam, QStringList()).toList();
    mws.SetContactsSplitterState(VarListToIntList(csvl));

    QVariantList msvl = settings.value(mainSplitterParam, QVariantList()).toList();
    mws.SetMainWndSplitterState(VarListToIntList(msvl));

    mws.SetPreviousSearches(settings.value(prevSearchAttrValuesParam, QStringList()).toStringList());
    mws.SetContactsListHeaderState(settings.value(contactsHeaderStateParam, QByteArray()).toByteArray());
    mws.SetFiltersListHeaderState(settings.value(filtersHeaderStateParam, QByteArray()).toByteArray());
    mws.SetPropertiesListHeaderState(settings.value(propertiesHeaderStateParam, QByteArray()).toByteArray());
}

void AppSettingsQtFacilityKeeper::SaveMainWndSettings(const MainWndSettings & mws)
{
    auto IntListToVarList = [](QList<int> il) {
        QVariantList r; for (int sz : il) { r.append(sz); } return r;
    };

    using namespace MainWndSettingsStrings;
    QSettings settings(QApplication::organizationName(), QApplication::applicationName());
    QSettingsAutoGroup autoGroup(settings, sectionName);
    settings.setValue(filterTopLevelRuleParam, mws.GetFlagAllConditionsShouldBeMet());
    settings.setValue(wndMaximizedParam, mws.GetFlagMaximized());
    settings.setValue(wndPosParam, mws.GetGeometry());
    settings.setValue(contactSplitterParam, IntListToVarList(mws.GetContactsSplitterState()));
    settings.setValue(mainSplitterParam, IntListToVarList(mws.GetMainWndSplitterState()));
    settings.setValue(prevSearchAttrValuesParam, mws.GetPrevSearches());
    settings.setValue(contactsHeaderStateParam, mws.GetContactsListHeaderState());
    settings.setValue(filtersHeaderStateParam, mws.GetFiltersListHeaderState());
    settings.setValue(propertiesHeaderStateParam, mws.GetPropertiesListHeaderState());
}

namespace SearchFilterSettingsStrings
{
    const QString sectionName = "SearchFilter";
    const QString typeParam = "Type";
    const QString numFiltersParam = "numFilters";
    const QString codeParam = "Code";
    const QString conditionParam = "Condition";
    const QString valueParam = "Value";
}


void AppSettingsQtFacilityKeeper::LoadFilterListSettings(FilterListSettings & fls)
{
    using namespace SearchFilterSettingsStrings;
    QSettings settings(QApplication::organizationName(), QApplication::applicationName());
    QSettingsAutoGroup autoGroupExternal(settings, sectionName);
    int numFilters = settings.value(numFiltersParam).toInt();
    for (int i = 0; i < numFilters; ++i) {
        QSettingsAutoGroup autoEndGroupInternal(settings, QString::number(i));
        int iFilterCode = settings.value(codeParam).toInt();
        QString filterVal = settings.value(valueParam).toString();
        int iFilterType = settings.value(typeParam).toInt();
        int iFilterCond = settings.value(conditionParam).toInt();
        FilterType filterType = static_cast<FilterType>(iFilterType);

        if (FilterType::Composite == filterType) {
            CompositeFilterId attrId = static_cast<CompositeFilterId>(iFilterCode);
            FilterEntry fe{ attrId, filterVal, static_cast<FilterCondition>(iFilterCond) };
            fls.AddFilter(fe);
        }
        else if (FilterType::LdapAttr == filterType) {
            adbook::Attributes::AttrId attrId = static_cast<adbook::Attributes::AttrId>(iFilterCode);
            FilterEntry fe{ attrId, filterVal, static_cast<FilterCondition>(iFilterCond) };
            fls.AddFilter(fe);
        }
        else {
            throw adbook::HrError(E_INVALIDARG, L"filterType", __FUNCTIONW__);
        }
    }
}

void AppSettingsQtFacilityKeeper::SaveFilterListSettings(const FilterListSettings & fls)
{
    using namespace SearchFilterSettingsStrings;
    QSettings settings(QApplication::organizationName(), QApplication::applicationName());

    QSettingsAutoGroup autoEndGroupExternal(settings, sectionName);
    settings.setValue(numFiltersParam, fls.GetNumFilters());
    for (size_t i = 0, count = fls.GetNumFilters(); i < count; ++i) {
        QSettingsAutoGroup autoEndGroupInternal(settings, QString::number(i));
        const FilterEntry & fe = fls.GetFilter(i);
        settings.setValue(typeParam, std::underlying_type_t<FilterType>(fe.GetFilterType()));

        std::visit (
            [&settings](auto && arg) {
                using namespace SearchFilterSettingsStrings;
                using T = std::decay_t<decltype(arg)>;
                if constexpr (std::is_same_v<T, adbook::Attributes::AttrId>) {
                    settings.setValue(codeParam, std::underlying_type_t<T>(arg));
                }
                if constexpr (std::is_same_v<T, CompositeFilterId>) {
                    settings.setValue(codeParam, std::underlying_type_t<T>(arg));
                }
            },
            fe.GetFilterCode()
        );
        settings.setValue(conditionParam,  std::underlying_type_t<FilterCondition>(fe.GetCondition()));
        settings.setValue(valueParam, fe.GetValue());
    }
}

