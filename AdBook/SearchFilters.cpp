#include "stdafx.h"
#include "SearchFilters.h"

namespace
{

const wchar_t baseSectionName[] = L"base";
const wchar_t sfBaseSectionName[] = L"sfl";
const wchar_t numItemsName[] = L"count";
const wchar_t attrIdName[] = L"attrId";
const wchar_t ruleIdName[] = L"ruleId";
const wchar_t attrValName[] = L"attrVal";

}

void SearchFilters::Load()
{
    clear();
    auto app = AfxGetApp();
    UINT numItems = app->GetProfileIntW(baseSectionName, numItemsName, 0);
    for (UINT i = 0; i < numItems; ++i)
    {
        SearchFilter sf;
        CString sectionName = CString(sfBaseSectionName) + boost::lexical_cast<std::wstring>(i).c_str();
        sf.attrId = app->GetProfileIntW(sectionName, attrIdName, 0);
        sf.rule = boost::numeric_cast<adbook::LdapRequest::MathingRule>(
            app->GetProfileIntW(sectionName, ruleIdName, adbook::LdapRequest::InvalidMatchingRule)
            );
        sf.attrValue = app->GetProfileStringW(sectionName, attrValName);
        push_back(std::move(sf));
    }
}

void SearchFilters::Save()
{
    auto app = AfxGetApp();
    const size_t numItems = size();
    app->WriteProfileInt(baseSectionName, numItemsName, boost::numeric_cast<int>(numItems));
    UINT sfIndex = 0;
    for (const auto & sf : *this)
    {
        CString sectionName = CString(sfBaseSectionName) + 
            boost::lexical_cast<std::wstring>(sfIndex).c_str();
        VERIFY(app->WriteProfileInt(sectionName, attrIdName, sf.attrId));
        VERIFY(app->WriteProfileInt(sectionName, ruleIdName, sf.rule));
        VERIFY(app->WriteProfileStringW(sectionName, attrValName, sf.attrValue));
        ++sfIndex;
    }
}
