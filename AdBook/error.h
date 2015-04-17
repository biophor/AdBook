#pragma once

class Error : public std::exception
{
public:
    Error(const wchar_t * where = NULL)
    {
        if (where)
        {
            wcscpy_s(whereBuf_, where);
        }
    }
    
    virtual CString What() const = 0;
    CString Where() const
    {
        return whereBuf_;
    }
    
    virtual const char * what() const
    {
        return whatBuf_;
    }
    
protected:
    mutable wchar_t errorBuf_[512] = {};
    char whatBuf_[64] = {};
    wchar_t whereBuf_[128] = {};
};


class HrError : public Error
{
public:
    HrError(const HRESULT hr, const wchar_t * where = NULL) : Error(where), hr_(hr)
    {
        sprintf_s(whatBuf_, "0x%X.", hr);
    }
    
    virtual CString What() const;
    
private:
    HRESULT hr_;
};

class Win32Error : public Error
{
public:
    Win32Error(const DWORD dw, const wchar_t * where = NULL) : Error(where),  dw_(dw)
    {
        sprintf_s(whatBuf_, "0x%X.", dw);
    }

    virtual CString What() const;

private:
    DWORD dw_;
};
