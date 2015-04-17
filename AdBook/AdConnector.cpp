#include "stdafx.h"
#include "error.h"
#include "AdConnector.h"


AdConnector::AdConnector(const ConnectionSettings & cs): cs_(cs)
{
}

AdConnector::~AdConnector()
{
}

void AdConnector::Connect()
{
    if (IsConnected())
    {
        objectPtr_.Release();
    }
    CString ldapPathName = L"LDAP://";
    if (!cs_.CurrentDomain())
    {
        ldapPathName += cs_.GetDC() + L"/";
    }
    ldapPathName += L"rootDSE";
    
    HRESULT hr = S_OK;
    IADsPtr rootDsePtr;
    if (cs_.CurrentUserCredentials())
    {
        hr = ADsGetObject(ldapPathName, IID_IADs, reinterpret_cast<void **>(&rootDsePtr.p));
    }
    else
    {
        hr = ADsOpenObject(ldapPathName, cs_.GetLogin(), cs_.GetPassword(), ADS_SECURE_AUTHENTICATION, 
            IID_IADs, reinterpret_cast<void **>(&rootDsePtr.p));
    }
    if (FAILED(hr))
    {
        throw HrError(hr);
    }
    _variant_t defaultNamingContext;
    hr = rootDsePtr->Get(CComBSTR(L"defaultNamingContext"), &defaultNamingContext);
    if (FAILED(hr))
    {
        throw HrError(hr);
    }
    CString defaultNamingContextDN = defaultNamingContext.bstrVal;
    if (defaultNamingContextDN.IsEmpty())
    {
        throw HrError(E_UNEXPECTED);
    }
    VERIFY(ldapPathName.Replace(L"rootDSE", defaultNamingContextDN) >= 1);
    
    if (cs_.CurrentUserCredentials())
    {
        hr = ADsGetObject(ldapPathName, IID_IDirectoryObject, reinterpret_cast<void **>(&objectPtr_.p));
    }
    else
    {
        hr = ADsOpenObject(ldapPathName, cs_.GetLogin(), cs_.GetPassword(), ADS_SECURE_AUTHENTICATION,
                    IID_IDirectoryObject, reinterpret_cast<void **>(&objectPtr_.p));
    }
    if (FAILED(hr))
    {
        throw HrError(hr);
    }
}

bool AdConnector::IsConnected() const
{
    return !!objectPtr_;
}

IDirectoryObjectPtr AdConnector::GetDirectoryObject() const
{
    return objectPtr_;
}
