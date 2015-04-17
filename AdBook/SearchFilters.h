#pragma once

struct SearchFilter
{
    UINT attrId;
    adbook::LdapRequest::MathingRule rule;
    CString attrValue;
};

class SearchFilters: public std::list<SearchFilter>
{
public:
    void Load();
    void Save();
};

