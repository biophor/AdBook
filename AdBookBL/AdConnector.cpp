// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/*
Copyright (C) 2015-2021 Andrei Goncharov.

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
#include "error.h"
#include "adsi.h"
#include "shared.h"
#include "AdConnector.h"

namespace adbook
{

AdConnector::AdConnector() = default;
AdConnector::~AdConnector() = default;


bool IsLdapPath( const std::wstring & str )
{
    if (str.empty()) {
        return false;
    }
    IADsPathNamePtr adsName;
    HRESULT hr = adsName.CoCreateInstance(CLSID_Pathname, NULL, CLSCTX_INPROC_SERVER);
    if (FAILED(hr)) {
        throw HrError(hr, L"CoCreateInstance() failed." __FUNCTIONW__);
    }
    hr = adsName->Set(CComBSTR(str.c_str()), ADS_SETTYPE_FULL);
    return SUCCEEDED(hr);
}

std::wstring RetrieveDefaultNamingContextName (
    const ConnectionParams & connectionSettings
)
{
    std::wstring ldapPathName = L"LDAP://";
    const bool connectSpecifiedDomain = !connectionSettings.Get_ConnectDomainYouLoggedIn();
    if (connectSpecifiedDomain)
    {
        const std::wstring domainNameOrDc = Trim(connectionSettings.GetAddress());
        ldapPathName += domainNameOrDc + L"/";
    }
    HRESULT hr = S_OK;
    static wchar_t rootDse[] = L"rootDSE";
    ldapPathName += rootDse;
    IADsPtr rootDsePtr;
    if (connectionSettings.Get_ConnectAsCurrentUser())
    {
        ADsGetObjectWrp (
            ldapPathName,
            IID_IADs,
            reinterpret_cast<void **>(&rootDsePtr.p)
        );
    }
    else
    {
        ADsOpenObjectWrp (
            ldapPathName,
            connectionSettings.GetLogin(),
            connectionSettings.GetPassword(),
            IID_IADs,
            reinterpret_cast<void **>(&rootDsePtr.p)
        );
    }

    _variant_t defaultNamingContext;
    hr = rootDsePtr->Get(CComBSTR(L"defaultNamingContext"), &defaultNamingContext);
    if (FAILED(hr)) {
        HR_ERROR(hr);
    }
    std::wstring defaultNamingContextDN = defaultNamingContext.bstrVal;
    if (defaultNamingContextDN.empty()) {
        HR_ERROR(E_UNEXPECTED);
    }

    size_t rootDsePos = ldapPathName.rfind(rootDse);
    if (rootDsePos != std::wstring::npos) {
        ldapPathName.replace(rootDsePos, _countof(rootDse), defaultNamingContextDN);
    }
    return defaultNamingContext.bstrVal;
}

std::wstring ReplaceDnInLdapPath (
    const std::wstring & ldapPath,
    const std::wstring & distinguishedName
)
{
    if (ldapPath.empty() || (ldapPath != Trim(ldapPath))) {
        throw HrError(E_INVALIDARG, L"ldapPath", __FUNCTIONW__);
    }

    if (distinguishedName.empty() || (distinguishedName != Trim(distinguishedName))) {
        throw HrError(E_INVALIDARG, L"distinguishedName", __FUNCTIONW__);
    }
    const bool ldapPathContainsDn = (ldapPath.find(L'=') != std::wstring::npos);

    std::wstring resultLdapPath;
    if (ldapPathContainsDn) {
        resultLdapPath = ldapPath.substr(0, ldapPath.rfind(L'/')+1) + distinguishedName;
    }
    else
    {
        if (ldapPath.back() == L'/') {
            resultLdapPath = ldapPath + distinguishedName;
        }
        else {
            resultLdapPath = ldapPath + L'/' + distinguishedName;
        }
    }
    return resultLdapPath;
}

std::wstring BuildLdapPath (
    const ConnectionParams & connectionSettings,
    const std::wstring & distinguishedName
)
{
    if (distinguishedName.empty() || (distinguishedName != Trim(distinguishedName))) {
        throw HrError(E_INVALIDARG, L"distinguishedName", __FUNCTIONW__);
    }

    std::wstring ldapPath = L"LDAP://";
    const bool addressSpecified = !connectionSettings.Get_ConnectDomainYouLoggedIn();

    if (addressSpecified)
    {
        const std::wstring address = Trim( connectionSettings.GetAddress() );
        if (address.empty()) {
            throw HrError(E_INVALIDARG, L"address", __FUNCTIONW__);
        }
        if (IsLdapPath(address)) {
            return ReplaceDnInLdapPath(address, distinguishedName);
        }
        ldapPath += address + L"/";
    }
    ldapPath += distinguishedName;
    return ldapPath;
}

std::wstring BuildLdapPath (
    const ConnectionParams & connectionParams
)
{
    std::wstring ldapPath = L"LDAP://";
    const bool addressSpecified = !connectionParams.Get_ConnectDomainYouLoggedIn();

    if (addressSpecified)
    {
        const std::wstring address = Trim(connectionParams.GetAddress());
        if (address.empty()) {
            throw HrError(E_INVALIDARG, L"address", __FUNCTIONW__);
        }
        if (IsLdapPath(address)) {
            return address;
        }
        ldapPath += address + L"/";
    }
    const std::wstring defaultNamingContext = RetrieveDefaultNamingContextName(connectionParams);
    ldapPath += defaultNamingContext;
    return ldapPath;
}

void AdConnector::Connect (
    const ConnectionParams & connectionParams
)
{
    if (IsConnected()) {
        _objectPtr.Release();
    }
    const std::wstring ldapPath = BuildLdapPath( connectionParams );

    if (connectionParams.Get_ConnectAsCurrentUser()) {
        _objectPtr = ADsGetDirectoryObject( ldapPath );
    }
    else {
        _objectPtr = ADsOpenDirectoryObject (
            ldapPath,
            connectionParams.GetLogin(),
            connectionParams.GetPassword()
        );
    }
    _connectionParams = connectionParams;
    _distinguishedName.clear();
}

void AdConnector::Connect (
    const ConnectionParams & connectionParams,
    const std::wstring & distinguishedName
)
{
    if (distinguishedName.empty() || (distinguishedName != Trim(distinguishedName))) {
        throw HrError(E_INVALIDARG, L"distinguishedName", __FUNCTIONW__);
    }
    if (IsConnected()) {
        _objectPtr.Release();
    }
    const std::wstring ldapPath = BuildLdapPath( connectionParams, distinguishedName);

    if (connectionParams.Get_ConnectAsCurrentUser()) {
        _objectPtr = ADsGetDirectoryObject( ldapPath );
    }
    else {
        _objectPtr = ADsOpenDirectoryObject(
            ldapPath,
            connectionParams.GetLogin(),
            connectionParams.GetPassword()
        );
    }
    _connectionParams = connectionParams;
    _distinguishedName = distinguishedName;
}

void AdConnector::Disconnect() {
    _objectPtr.Release();
}

bool AdConnector::IsConnected() const {
    return !!_objectPtr;
}

IDirectoryObjectPtr AdConnector::GetDirectoryObject() const {
    return _objectPtr;
}

void AdConnector::UploadStringAttr (
    const std::wstring & attrName,
    const std::wstring & attrVal
)
{
    if (attrName.empty() || (attrName != Trim(attrName))) {
        throw HrError(E_INVALIDARG, L"attrName", __FUNCTIONW__);
    }
    if (!IsConnected()) {
        HR_ERROR(E_UNEXPECTED);
    }
    ADSVALUE adsValue{};
    DWORD numChanged = 0;
    WcharBuf attrNameBuf = ToWcharBuf(attrName);

    if (!attrVal.empty())
    {
        ADS_ATTR_INFO attrInfo[] = {
            { &attrNameBuf.at(0), ADS_ATTR_UPDATE, ADSTYPE_CASE_IGNORE_STRING, &adsValue, 1 }
        };
        adsValue.dwType = ADSTYPE_CASE_IGNORE_STRING;
        WcharBuf newAttrValue = ToWcharBuf(attrVal);
        adsValue.CaseIgnoreString = &newAttrValue.at(0);
        SetObjectAttributesWrp(&attrInfo[0], _countof(attrInfo), &numChanged);
    }
    else
    {
        ADS_ATTR_INFO attrInfo[] = {
            { &attrNameBuf.at(0), ADS_ATTR_CLEAR, ADSTYPE_CASE_IGNORE_STRING, NULL, 1 }
        };
        adsValue.dwType = ADSTYPE_CASE_IGNORE_STRING;
        SetObjectAttributesWrp(&attrInfo[0], _countof(attrInfo), &numChanged);
    }

    if (numChanged != 1) {
        throw HrError(E_UNEXPECTED, L"numChanged != 1", __FUNCTIONW__);
    }
}

std::wstring AdConnector::DownloadStringAttr (
    const std::wstring & attrName
)
{
    if (attrName.empty() || (attrName != Trim(attrName))) {
        throw HrError(E_INVALIDARG, L"attrName", __FUNCTIONW__);
    }
    if (!IsConnected()) {
        HR_ERROR(E_UNEXPECTED);
    }
    ADS_ATTR_INFO * attrInfo = NULL;
    DWORD numReturned = 0;
    WcharBuf wAttrName = ToWcharBuf(attrName);
    LPWSTR attrNames[] = { &wAttrName.at(0) };
    GetObjectAttributesWrp(attrNames, _countof(attrNames), &attrInfo, &numReturned);

    std::unique_ptr<ADS_ATTR_INFO, BOOL  (__stdcall *)(LPVOID)> freeAdsMemPtr(attrInfo, &FreeADsMem);
    if (1 != numReturned)
    {
        // attrName might be incorrect.
        // GetObjectAttributesWrp() does not throw an exception if attrName is incorrect.
        CheckAttrNameByClearingAttribute(attrName, ADSTYPE_PROV_SPECIFIC);
        return {};
    }

    switch (attrInfo[0].dwADsType)
    {
    case ADSTYPE_DN_STRING:
    case ADSTYPE_CASE_EXACT_STRING:
    case ADSTYPE_CASE_IGNORE_STRING:
    case ADSTYPE_PRINTABLE_STRING:
    case ADSTYPE_NUMERIC_STRING:
    {
        if (auto & str = attrInfo[0].pADsValues->CaseIgnoreString; str != nullptr) {
            return std::wstring(str);
        }
        return {};
    }

    case ADSTYPE_PROV_SPECIFIC:
    {
        if (auto & o = attrInfo[0].pADsValues->ProviderSpecific; o.lpValue && o.dwLength) {
            return std::wstring(
                reinterpret_cast<LPCWSTR>(o.lpValue),
                static_cast<size_t>(o.dwLength) / sizeof(wchar_t)
            );
        }
        return {};
    }
    default:
        throw HrError(E_UNEXPECTED, L"unsupported dwADsType", __FUNCTIONW__);
    }
}

void AdConnector::UploadBinaryAttr (
    const std::wstring & attrName,
    const BinaryAttrVal & bav
)
{
    if (attrName.empty() || (attrName != Trim(attrName))) {
        throw HrError(E_INVALIDARG, L"attrName", __FUNCTIONW__);
    }
    if (!IsConnected()) {
        HR_ERROR(E_UNEXPECTED);
    }
    ADSVALUE adsValue{};
    DWORD numChanged = 0;
    WcharBuf attrNameBuf = ToWcharBuf(attrName);

    if (!bav.empty())
    {
        ADS_ATTR_INFO attrInfo[] = {
            { &attrNameBuf.at(0), ADS_ATTR_UPDATE, ADSTYPE_OCTET_STRING, &adsValue, 1 }
        };
        adsValue.dwType = ADSTYPE_OCTET_STRING;
        BinaryAttrVal newAttrValue = bav;
        adsValue.OctetString.lpValue = &newAttrValue.at(0);
        adsValue.OctetString.dwLength = static_cast<DWORD>(newAttrValue.size());
        SetObjectAttributesWrp(&attrInfo[0], _countof(attrInfo), &numChanged);
    }
    else
    {
        ADS_ATTR_INFO attrInfo[] = {
            { &attrNameBuf.at(0), ADS_ATTR_CLEAR, ADSTYPE_OCTET_STRING, NULL, 1 }
        };
        adsValue.dwType = ADSTYPE_OCTET_STRING;
        SetObjectAttributesWrp(&attrInfo[0], _countof(attrInfo), &numChanged);
    }

    if (numChanged != 1) {
        throw HrError(E_UNEXPECTED, L"numChanged != 1", __FUNCTIONW__);
    }
}

BinaryAttrVal AdConnector::DownloadBinaryAttr (
    const std::wstring & attrName
)
{
    if (attrName.empty() || (attrName != Trim(attrName))) {
        throw HrError(E_INVALIDARG, L"attrName", __FUNCTIONW__);
    }
    if (!IsConnected()) {
        HR_ERROR(E_UNEXPECTED);
    }
    ADS_ATTR_INFO * attrInfo = NULL;
    DWORD numReturned = 0;
    WcharBuf wAttrName = ToWcharBuf(attrName);
    LPWSTR attrNames[] = { &wAttrName.at(0) };
    GetObjectAttributesWrp(attrNames, _countof(attrNames), &attrInfo, &numReturned);

    std::unique_ptr<ADS_ATTR_INFO, BOOL  (__stdcall *)(LPVOID)> freeAdsMemPtr(attrInfo, &FreeADsMem);

    if (1 != numReturned)
    {
        // attrName might be incorrect.
        // GetObjectAttributesWrp() does not throw an exception if attrName is incorrect.
        CheckAttrNameByClearingAttribute(attrName, ADSTYPE_OCTET_STRING);
        return {};
    }

    switch (attrInfo[0].dwADsType)
    {
    case ADSTYPE_OCTET_STRING:
    {
        if (const auto & o = attrInfo[0].pADsValues->OctetString; o.lpValue && o.dwLength) {
            return BinaryAttrVal(o.lpValue, o.lpValue + static_cast<size_t>(o.dwLength));
        }
        else {
            return BinaryAttrVal{};
        }
    }
    case ADSTYPE_PROV_SPECIFIC:
    {
        if (const auto & o = attrInfo[0].pADsValues->ProviderSpecific; o.lpValue && o.dwLength) {
            return BinaryAttrVal(o.lpValue, o.lpValue + static_cast<size_t>(o.dwLength));
        }
        else {
            return BinaryAttrVal{};
        }
    }
    default:
        throw HrError(E_UNEXPECTED, L"unsupported dwADsType", __FUNCTIONW__);
    }
}

std::wstring AdConnector::GetLdapPath()
{
    if (!IsConnected()) {
        HR_ERROR(E_UNEXPECTED);
    }
    IADsPtr ads = _objectPtr;
    CComBSTR personLdapPath;
    HRESULT hr = ads->get_ADsPath(&personLdapPath);
    if (FAILED(hr)) {
        throw HrError(hr, L"IADsPtr::get_ADsPath() failed", __FUNCTIONW__);
    }
    return std::wstring(personLdapPath);
}

std::wstring AdConnector::GetRDN()
{
    if (!IsConnected()) {
        HR_ERROR(E_UNEXPECTED);
    }
    IADsPtr ads = _objectPtr;
    CComBSTR name;
    HRESULT hr = ads->get_Name(&name);
    if (FAILED(hr)) {
        throw HrError(hr, L"IADsPtr::get_Name() failed", __FUNCTIONW__);
    }
    return std::wstring(name);
}

IDirectoryObjectPtr AdConnector::GetParentObject()
{
    if (!IsConnected()) {
        HR_ERROR(E_UNEXPECTED);
    }
    const std::wstring parentLdapPath = GetParentLdapPath();
    IDirectoryObjectPtr parent;

    if (_connectionParams.Get_ConnectAsCurrentUser()) {
        parent = ADsGetDirectoryObject(parentLdapPath);
    }
    else {
        parent = ADsOpenDirectoryObject (
            parentLdapPath,
            _connectionParams.GetLogin(),
            _connectionParams.GetPassword()
        );
    }
    return parent;
}

void AdConnector::Rename (
    const std::wstring & newName
)
{
    if (newName.empty() || (newName != Trim(newName))) {
        throw HrError(E_INVALIDARG, L"newName", __FUNCTIONW__);
    }
    if (!IsConnected()) {
        HR_ERROR(E_UNEXPECTED);
    }

    IADsContainerPtr parent = GetParentObject();
    std::wstring personLdapPath = GetLdapPath();

    Disconnect();

    std::wstring newPersonRDN(L"CN=");
    newPersonRDN += newName;
    MyIDispatchPtr disp;

    const HRESULT hr = parent->MoveHere (
        CComBSTR(personLdapPath.c_str()),
        CComBSTR(newPersonRDN.c_str()),
        &disp
    );
    if (FAILED(hr)) {
        throw HrError(hr, L"IADsContainerPtr::MoveHere() failed", __FUNCTIONW__);
    }
    _objectPtr = disp;
}

std::wstring AdConnector::GetParentLdapPath()
{
    if (!IsConnected()) {
        HR_ERROR(E_UNEXPECTED);
    }
    IADsPtr ads = _objectPtr;
    CComBSTR parentLdapPath;
    HRESULT hr = ads->get_Parent(&parentLdapPath);
    if (FAILED(hr)) {
        throw HrError(hr, L"IADsPtr::get_Parent() failed", __FUNCTIONW__);
    }
    return ToWstring(parentLdapPath);
}

void AdConnector::GetObjectAttributesWrp (
    LPWSTR * attributeNames,
    DWORD numberAttributes,
    PADS_ATTR_INFO * attributeEntries,
    DWORD * numAttributesReturned
)
{
    if (!IsConnected()) {
        HR_ERROR(E_UNEXPECTED);
    }
    const HRESULT hr = _objectPtr->GetObjectAttributes (
        attributeNames,
        numberAttributes,
        attributeEntries,
        numAttributesReturned
    );
    if (FAILED(hr)) {
        throw HrError (
            hr,
            L"IDirectoryObject::GetObjectAttributes() failed",
            __FUNCTIONW__
        );
    }
}

void AdConnector::SetObjectAttributesWrp (
    PADS_ATTR_INFO attributeEntries,
    DWORD numAttributes,
    DWORD * numAttributesModified
)
{
    if (!IsConnected()) {
        HR_ERROR(E_UNEXPECTED);
    }
    const HRESULT hr = _objectPtr->SetObjectAttributes (
        attributeEntries, numAttributes, numAttributesModified
    );
    if (FAILED(hr)) {
        throw HrError (
            hr,
            L"IDirectoryObject::SetObjectAttributes() failed",
            __FUNCTIONW__
        );
    }
}

void AdConnector::CheckAttrNameByClearingAttribute (
    const std::wstring & attrName,
    ADSTYPE attrYype
)
{
    if (!IsConnected()) {
        HR_ERROR(E_UNEXPECTED);
    }
    if (attrName.empty() || (Trim(attrName) != attrName)) {
        throw HrError(E_INVALIDARG, L"attrName", __FUNCTIONW__);
    }
    WcharBuf wAttrName = ToWcharBuf(attrName);
    ADS_ATTR_INFO attrInfoToTest[] = {
        { &wAttrName.at(0), ADS_ATTR_CLEAR, attrYype, NULL, 1 }
    };
    DWORD numChanged = 0;
    const HRESULT hr = _objectPtr->SetObjectAttributes (
        &attrInfoToTest[0],
        _countof(attrInfoToTest),
        &numChanged
    );
    if (hr == HRESULT_FROM_WIN32(ERROR_DS_NO_ATTRIBUTE_OR_VALUE)) {
        // attrName is invalid
        throw HrError(E_INVALIDARG, L"attrName", __FUNCTIONW__);
    }
}

}   // namespace adbook

