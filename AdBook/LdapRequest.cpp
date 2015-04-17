#include "stdafx.h"
#include "error.h"
#include "LdapRequest.h"

const std::map<LdapRequest::AttrId, const wchar_t*> LdapRequest::attrInfo =
{
    { CommonName, L"cn" },
    { Email, L"mail" },
    { Company, L"company" },
    { Title, L"title" },
    { Department, L"department" },
    { State, L"st" },
    { EmpId, L"employeeID" },
    { Locality, L"l" }
};


LdapRequest::LdapRequest()
{
}


LdapRequest::~LdapRequest()
{
}


void LdapRequest::AddRule(const AttrId attrId, const MathingRule rule, const CString & value)
{
    const CString & attrName = attrInfo.at(attrId);    
    AddRule(attrName, rule, value);
}

void LdapRequest::AddRule(const CString & attrName, const MathingRule rule, const CString & value)
{
    CString lan = attrName;
    lan.Trim();
    if (lan.IsEmpty())
    {
        throw HrError(E_INVALIDARG);
    }
    CString lv = value;
    lv.Trim();
    if (!lv.IsEmpty())
    {
        lv.Replace(L"\\", L"\\5c");
        lv.Replace(L"*", L"\\2a");
        lv.Replace(L"/", L"\\2f");
        lv.Replace(L"\0", L"\\00");
        lv.Replace(L"(", L"\\28");
        lv.Replace(L")", L"\\29");
    }
    switch (rule)
    {
    case Contains: 
            lv = CString(L"*") + lv + L"*";
        break;
    case BeginWith:
            lv = lv + L"*";
        break;
    case EndWith:
            lv = CString(L"*") + lv;
        break;
    }
    CString s = CString(L"(") + lan + L"=" + lv + L")";
    request_ = request_ + s;
}

void LdapRequest::AddOR()
{
    request_ = CString(L"(|") + request_ + L")";
}

void LdapRequest::AddAND()
{
    request_ = CString(L"(&") + request_ + L")";
}

void LdapRequest::AddNOT()
{
    request_ = CString(L"(!") + request_ + L")";
}

CString LdapRequest::Get() const
{
    return request_;
}

void LdapRequest::Clear()
{
    request_.Empty();
}
