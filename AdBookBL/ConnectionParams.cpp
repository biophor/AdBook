#include "stdafx.h"
#include "ConnectionParams.h"

namespace adbook
{

std::wstring ConnectionParams::GetDC() const
{
    return dc_;
}

void ConnectionParams::CurrentUserCredentials(bool use) noexcept
{
    currentUserCred_ = use;
}

bool ConnectionParams::CurrentUserCredentials() const noexcept
{
    return currentUserCred_;
}

void ConnectionParams::CurrentDomain(const bool currentDomain) noexcept
{
    currentDomain_ = currentDomain;
}

bool ConnectionParams::CurrentDomain() const noexcept
{
    return currentDomain_;
}

std::wstring ConnectionParams::GetLogin() const
{
    return login_;
}

std::wstring ConnectionParams::GetPassword() const
{
    return password_;
}

} // namespace adbook
