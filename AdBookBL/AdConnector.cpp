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
#include "error.h"
#include "adsi.h"
#include "shared.h"
#include "AdConnector.h"

namespace adbook
{


AdConnector::AdConnector() = default;

AdConnector::~AdConnector() = default;


bool IsLdapPath(const std::wstring & str)
{
    if (str.empty())
    {
        return false;
    }
    IADsPathNamePtr adsName;
    if (SUCCEEDED(adsName.CoCreateInstance(CLSID_Pathname, NULL, CLSCTX_INPROC_SERVER)))
    {
        HRESULT hr = adsName->Set(CComBSTR(str.c_str()), ADS_SETTYPE_FULL);
        return SUCCEEDED(hr);
    }

    return false;
}

std::wstring RetrieveDefaultNamingContextName(const ConnectionParams & connectionSettings)
{
    std::wstring domainNameOrDc = connectionSettings.GetDomainController();
    boost::trim(domainNameOrDc);
    if (domainNameOrDc.empty())
    {
        HR_ERROR(E_INVALIDARG);
    }
    std::wstring ldapPathName = L"LDAP://";
    if (!connectionSettings.CurrentDomain())
    {
        ldapPathName += domainNameOrDc + L"/";
    }
    HRESULT hr = S_OK;
    wchar_t rootDse[] = L"rootDSE";
    ldapPathName += rootDse;
    IADsPtr rootDsePtr;
    if (connectionSettings.IsCurrentUserCredentialsUsed())
    {
        hr = ADsGetObject(
            ldapPathName.c_str(),
            IID_IADs,
            reinterpret_cast<void **>(&rootDsePtr.p)
        );
    }
    else
    {
        hr = ADsOpenObject(
            ldapPathName.c_str(),
            connectionSettings.GetLogin().c_str(),
            connectionSettings.GetPassword().c_str(),
            ADS_SECURE_AUTHENTICATION, IID_IADs,
            reinterpret_cast<void **>(&rootDsePtr.p)
        );
    }
    if (FAILED(hr))
    {
        HR_ERROR(hr);
    }
    _variant_t defaultNamingContext;
    hr = rootDsePtr->Get(CComBSTR(L"defaultNamingContext"), &defaultNamingContext);
    if (FAILED(hr))
    {
        HR_ERROR(hr);
    }
    std::wstring defaultNamingContextDN = defaultNamingContext.bstrVal;
    if (defaultNamingContextDN.empty())
    {
        HR_ERROR(E_UNEXPECTED);
    }

    size_t rootDsePos = ldapPathName.rfind(rootDse);
    if (rootDsePos != std::wstring::npos)
    {
        ldapPathName.replace(rootDsePos, _countof(rootDse), defaultNamingContextDN);
    }
    return defaultNamingContext.bstrVal;
}

std::wstring ReplaceDnInLdapPath(const std::wstring & ldapPath, const std::wstring & dn)
{
    if (ldapPath.empty())
    {
        HR_ERROR(E_INVALIDARG);
    }
    if (dn.empty())
    {
        return ldapPath;
    }
    const bool ldapPathContainsDn = (ldapPath.find(L'=') != std::wstring::npos);

    std::wstring resultLdapPath;
    if (ldapPathContainsDn)
    {
        resultLdapPath = ldapPath.substr(0, ldapPath.rfind(L'/')+1) + dn;
    }
    else
    {
        if (ldapPath.back() == L'/')
        {
            resultLdapPath = ldapPath + dn;
        }
        else
        {
            resultLdapPath = ldapPath + L'/' + dn;
        }
    }
    return resultLdapPath;
}


std::wstring BuildLdapPath(const ConnectionParams & connectionSettings, const std::wstring & dn)
{
    std::wstring domainName_or_DC_or_LdapPath = connectionSettings.GetDomainController();
    boost::trim(domainName_or_DC_or_LdapPath);

    if (IsLdapPath(domainName_or_DC_or_LdapPath))
    {
        return ReplaceDnInLdapPath(domainName_or_DC_or_LdapPath, dn);
    }

    std::wstring ldapPathName = L"LDAP://";
    if (false == connectionSettings.CurrentDomain())
    {
        ldapPathName += domainName_or_DC_or_LdapPath + L"/";
    }

    std::wstring distinguishedName = dn;
    boost::trim(distinguishedName);

    if (distinguishedName.empty())
    {
        distinguishedName = RetrieveDefaultNamingContextName(connectionSettings);
    }
    ldapPathName += distinguishedName;
    return ldapPathName;
}

void AdConnector::Connect(const ConnectionParams & connectionParams)
{
    Connect(connectionParams, L"");
}

void AdConnector::Connect(const ConnectionParams & connectionSettings, const std::wstring & dn)
{
    if (IsConnected())
    {
        objectPtr_.Release();
    }

    std::wstring ldapPathName = BuildLdapPath(connectionSettings, dn);

    HRESULT hr = S_OK;
    if (connectionSettings.IsCurrentUserCredentialsUsed())
    {
        hr = ADsGetObject(
            ldapPathName.c_str(),
            IID_IDirectoryObject,
            reinterpret_cast<void **>(&objectPtr_.p)
        );
    }
    else
    {
        hr = ADsOpenObject(ldapPathName.c_str(),
            connectionSettings.GetLogin().c_str(),
            connectionSettings.GetPassword().c_str(),
            ADS_SECURE_AUTHENTICATION,
            IID_IDirectoryObject,
            reinterpret_cast<void **>(&objectPtr_.p)
        );
    }
    if (FAILED(hr))
    {
        HR_ERROR(hr);
    }
    connectionSettings_ = connectionSettings;
    distinguishedName_ = dn;
}

void AdConnector::Disconnect()
{
    objectPtr_.Release();
}

bool AdConnector::IsConnected() const
{
    return !!objectPtr_;
}

IDirectoryObjectPtr AdConnector::GetDirectoryObject() const
{
    return objectPtr_;
}

void AdConnector::UploadStringAttr(const std::wstring & attrName, const std::wstring & attrVal)
{
    if (!IsConnected())
    {
        HR_ERROR(E_UNEXPECTED);
    }
    DWORD numChanged = 0;

    WcharBuf attrNameBuf = ToWcharBuf(attrName);
    if (!attrVal.empty())
    {
        ADSVALUE adsValue{};
        ADS_ATTR_INFO attrInfo[] = { { &attrNameBuf.at(0), ADS_ATTR_UPDATE, ADSTYPE_CASE_IGNORE_STRING, &adsValue, 1 } };
        adsValue.dwType = ADSTYPE_CASE_IGNORE_STRING;
        WcharBuf newAttrValue = ToWcharBuf(attrVal);
        adsValue.CaseIgnoreString = &newAttrValue.at(0);
        const HRESULT hr = objectPtr_->SetObjectAttributes(&attrInfo[0], _countof(attrInfo), &numChanged);
        if (FAILED(hr))
        {
            HR_ERROR(hr);
        }
    }
    else
    {
        ADSVALUE adsValue{};
        ADS_ATTR_INFO attrInfo[] = { { &attrNameBuf.at(0), ADS_ATTR_CLEAR, ADSTYPE_CASE_IGNORE_STRING, NULL, 1 } };
        adsValue.dwType = ADSTYPE_CASE_IGNORE_STRING;
        const HRESULT hr = objectPtr_->SetObjectAttributes(&attrInfo[0], _countof(attrInfo), &numChanged);
        if (FAILED(hr))
        {
            HR_ERROR(hr);
        }
    }
}

std::wstring AdConnector::DownloadStringAttr(const std::wstring & attrName)
{
    if (!IsConnected())
    {
        HR_ERROR(E_UNEXPECTED);
    }
    ADS_ATTR_INFO * attrInfo = NULL;
    DWORD numReturned = 0;
    WcharBuf wAttrName = ToWcharBuf(attrName);
    LPWSTR attrNames[] = { &wAttrName.at(0) };
    HRESULT hr = objectPtr_->GetObjectAttributes(attrNames, _countof(attrNames), &attrInfo, &numReturned);
    if (FAILED(hr))
    {
        HR_ERROR(hr);
    }
    std::unique_ptr<ADS_ATTR_INFO, BOOL  (__stdcall *)(LPVOID)> freeAdsMemPtr(attrInfo, &FreeADsMem);
    if (1 != numReturned)
    {
        // attr value is not set or attr doesn't exist
        // we need to differentiate these two cases.
        // we can try to clear the value of the attribute.
        // if attr doesn't exist we will get the ERROR_DS_NO_ATTRIBUTE_OR_VALUE error code
        ADS_ATTR_INFO attrInfoToTest[] = { { &wAttrName.at(0), ADS_ATTR_CLEAR, ADSTYPE_CASE_IGNORE_STRING, NULL, 1 } };
        DWORD numChanged = 0;
        hr = objectPtr_->SetObjectAttributes(&attrInfoToTest[0], _countof(attrInfoToTest), &numChanged);
        if (hr == HRESULT_FROM_WIN32(ERROR_DS_NO_ATTRIBUTE_OR_VALUE)) {
            // attrName is invalid
            HR_ERROR(hr);
        }
        // attrValue is not set or access to the value is denied.
        return {};
    }

    switch (attrInfo[0].dwADsType)
    {
    case ADSTYPE_DN_STRING:
    case ADSTYPE_CASE_EXACT_STRING:
    case ADSTYPE_CASE_IGNORE_STRING:
    case ADSTYPE_PRINTABLE_STRING:
    case ADSTYPE_NUMERIC_STRING:
        return attrInfo[0].pADsValues->CaseIgnoreString;

    case ADSTYPE_PROV_SPECIFIC:
        return std::wstring(
            reinterpret_cast<LPCWSTR>(attrInfo[0].pADsValues->ProviderSpecific.lpValue),
            static_cast<size_t>(attrInfo[0].pADsValues->ProviderSpecific.dwLength) / sizeof(wchar_t)
        );

    default:
        HR_ERROR(E_INVALIDARG);
    }
}

void AdConnector::UploadBinaryAttr(const std::wstring & attrName, const BinaryAttrVal & bav)
{
    if (!IsConnected())
    {
        HR_ERROR(E_UNEXPECTED);
    }
    DWORD numChanged = 0;

    WcharBuf attrNameBuf = ToWcharBuf(attrName);
    if (!bav.empty())
    {
        ADSVALUE adsValue{};
        ADS_ATTR_INFO attrInfo[] = { { &attrNameBuf.at(0), ADS_ATTR_UPDATE, ADSTYPE_OCTET_STRING, &adsValue, 1 } };
        adsValue.dwType = ADSTYPE_OCTET_STRING;
        BinaryAttrVal newAttrValue = bav;
        adsValue.OctetString.lpValue = &newAttrValue.at(0);
        adsValue.OctetString.dwLength = boost::numeric_cast<DWORD>(newAttrValue.size());
        const HRESULT hr = objectPtr_->SetObjectAttributes(&attrInfo[0], _countof(attrInfo), &numChanged);
        if (FAILED(hr))
        {
            HR_ERROR(hr);
        }
    }
    else
    {
        ADSVALUE adsValue{};
        ADS_ATTR_INFO attrInfo[] = { { &attrNameBuf.at(0), ADS_ATTR_CLEAR, ADSTYPE_OCTET_STRING, NULL, 1 } };
        adsValue.dwType = ADSTYPE_OCTET_STRING;
        const HRESULT hr = objectPtr_->SetObjectAttributes(&attrInfo[0], _countof(attrInfo), &numChanged);
        if (FAILED(hr))
        {
            HR_ERROR(hr);
        }
    }
}

BinaryAttrVal AdConnector::DownloadBinaryAttr(const std::wstring & attrName)
{
    if (!IsConnected())
    {
        HR_ERROR(E_UNEXPECTED);
    }
    ADS_ATTR_INFO * attrInfo = NULL;
    DWORD numReturned = 0;
    WcharBuf wAttrName = ToWcharBuf(attrName);
    LPWSTR attrNames[] = { &wAttrName.at(0) };
    const HRESULT hr = objectPtr_->GetObjectAttributes(attrNames, _countof(attrNames), &attrInfo, &numReturned);
    if (FAILED(hr))
    {
        HR_ERROR(hr);
    }
    std::shared_ptr<ADS_ATTR_INFO> freeAdsMemPtr(attrInfo, FreeADsMem);
    if (1 != numReturned)
    {
        HR_ERROR(ERROR_READ_FAULT);
    }
    switch (attrInfo[0].dwADsType)
    {
    case ADSTYPE_OCTET_STRING:
    {
        const auto & o = attrInfo[0].pADsValues->OctetString;
        return BinaryAttrVal(o.lpValue, o.lpValue + boost::numeric_cast<size_t>(o.dwLength));
    }
    case ADSTYPE_PROV_SPECIFIC:
    {
        const auto & o = attrInfo[0].pADsValues->ProviderSpecific;
        return BinaryAttrVal(o.lpValue, o.lpValue + boost::numeric_cast<size_t>(o.dwLength));
    }
    default:
        HR_ERROR(E_INVALIDARG);
    }
}

std::wstring AdConnector::GetLdapPath()
{
    if (!IsConnected())
    {
        HR_ERROR(E_UNEXPECTED);
    }
    IADsPtr ads = objectPtr_;
    CComBSTR personLdapPath;
    HRESULT hr = ads->get_ADsPath(&personLdapPath);
    if (FAILED(hr))
    {
        HR_ERROR(hr);
    }
    return std::wstring(personLdapPath);
}

std::wstring AdConnector::GetRDN()
{
    if (!IsConnected())
    {
        HR_ERROR(E_UNEXPECTED);
    }
    IADsPtr ads = objectPtr_;
    CComBSTR name;
    HRESULT hr = ads->get_Name(&name);
    if (FAILED(hr))
    {
        HR_ERROR(hr);
    }
    return std::wstring(name);
}

IDirectoryObjectPtr AdConnector::GetParentObject()
{
    if (!IsConnected())
    {
        HR_ERROR(E_UNEXPECTED);
    }
    IADsPtr ads = objectPtr_;
    CComBSTR parentLdapPath;
    HRESULT hr = ads->get_Parent(&parentLdapPath);
    if (FAILED(hr))
    {
        HR_ERROR(hr);
    }
    IDirectoryObjectPtr parent;
    if (connectionSettings_.IsCurrentUserCredentialsUsed())
    {
        hr = ADsGetObject(parentLdapPath, IID_IDirectoryObject, reinterpret_cast<void**>(&parent.p));
    }
    else
    {
        hr = ADsOpenObject(parentLdapPath, connectionSettings_.GetLogin().c_str(), connectionSettings_.GetPassword().c_str(),
            ADS_SECURE_AUTHENTICATION, IID_IDirectoryObject, reinterpret_cast<void**>(&parent.p));
    }
    if (FAILED(hr))
    {
        HR_ERROR(hr);
    }
    return parent;
}

void AdConnector::Rename(const std::wstring & newName)
{
    if (!IsConnected())
    {
        HR_ERROR(E_UNEXPECTED);
    }
    std::wstring lNewName = newName;
    boost::trim(lNewName);
    if (lNewName.empty())
    {
        HR_ERROR(E_INVALIDARG);
    }
    IADsContainerPtr parent = GetParentObject();
    std::wstring personLdapPath = GetLdapPath();
    Disconnect();
    std::wstring newPersonRDN(L"CN=");
    newPersonRDN += newName;
    MyIDispatchPtr disp;
    const HRESULT hr = parent->MoveHere(CComBSTR(personLdapPath.c_str()),
                                        CComBSTR(newPersonRDN.c_str()), &disp);
    if (FAILED(hr))
    {
        HR_ERROR(hr);
    }
    objectPtr_ = disp;
}



}   // namespace adbook

