#pragma once
class LdapRequest
{
public:
    LdapRequest();
    ~LdapRequest();

    enum AttrId { CommonName, Email, Company, Title, Department, State, EmpId, Locality, AttrIdEnd };
    
    static const std::map<AttrId, const wchar_t*> attrInfo;
    
    enum MathingRule
    {
        ExactMatch = 1, Contains, BeginWith, EndWith
    };
    
    void AddRule(const AttrId attrId, const MathingRule rule, const CString & value);
    void AddRule(const CString & attrName, const MathingRule rule, const CString & value);
    void AddOR();  // before: (x=1)(y=2)    after: (|(x=1)(y=2))
    void AddAND(); // before: (x=1)(y=2)    after: (&(x=1)(y=2))
    void AddNOT(); // before: (x=1)         after: (!(x=1))
    CString Get() const;

    void Clear();
    
private:
    CString request_;
};

