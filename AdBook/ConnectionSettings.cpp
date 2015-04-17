#include "stdafx.h"
#include "ConnectionSettings.h"

ConnectionSettings::ConnectionSettings()
{

}

void ConnectionSettings::SetDC(const CString & dc)
{
    dc_ = dc;
}

CString ConnectionSettings::GetDC() const
{
    return dc_;
}

void ConnectionSettings::SetLogin(const CString & login)
{
    login_ = login;
}

void ConnectionSettings::SetPassword(const CString & password)
{
    password_ = password;
}

void ConnectionSettings::CurrentUserCredentials(bool use)
{
    currentUserCred_ = use;
}

bool ConnectionSettings::CurrentUserCredentials() const
{
    return currentUserCred_;
}

void ConnectionSettings::CurrentDomain(const bool currentDomain)
{
    currentDomain_ = currentDomain;
}

bool ConnectionSettings::CurrentDomain() const
{
    return currentDomain_;
}

CString ConnectionSettings::GetLogin() const
{
    return login_;
}

CString ConnectionSettings::GetPassword() const
{
    return password_;
}

void ConnectionSettings::ForgetPassword(const bool forget)
{
    forgetPassword_ = forget;
}

bool ConnectionSettings::ForgetPassword() const
{
    return forgetPassword_;
}

void ConnectionSettings::DisplayPassword(const bool display)
{
    displayPassword_ = display;
}

bool ConnectionSettings::DisplayPassword() const
{
    return displayPassword_;
}
