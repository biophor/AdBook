#pragma once
#include "AdBookBLExport.h"

namespace adbook
{

class ADBOOKBL_API ConnectionParams
{
public:    
    template <class T>
    void SetDC(T && dc);

    std::wstring GetDC() const;

    template <class T>
    void SetLogin(T && login);

    template <class T>
    void SetPassword(T && password);

    void CurrentUserCredentials(const bool currentUserCred) noexcept;
    bool CurrentUserCredentials() const noexcept;
    void CurrentDomain(const bool currentDomain) noexcept;
    bool CurrentDomain() const noexcept;
    std::wstring GetLogin() const;
    std::wstring GetPassword() const;

private:
    std::wstring dc_;    // domain controller name
    bool currentUserCred_ = true;
    bool currentDomain_ = true;
    bool displayPassword_ = false;
    bool forgetPassword_ = true;
    std::wstring login_, password_;
};

template <class T>
void ConnectionParams::SetDC(T && dc)
{
    dc_ = std::forward<T>(dc);
}

template <class T>
void ConnectionParams::SetLogin(T && login)
{
    login_ = std::forward<T>(login);
}

template <class T>
void ConnectionParams::SetPassword(T && password)
{
    password_ = std::forward<T>(password);
}


}

