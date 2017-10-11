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


#pragma once

#include "adsi.h"
#include "AdPersonDesc.h"
#include "AdBookBLExport.h"
#include "ConnectionParams.h"

namespace adbook
{
class ADBOOKBL_API AbstractAdSearcher
{
public:
    virtual ~AbstractAdSearcher() = 0;

    using OnNewItem = std::function<void(AdPersonDesc && item)>; // new item has been found.
    using OnStop = std::function<void()>;   // SearchThread is about to be completed.
    using OnStart = std::function<void()>;  // SearchThread has just been started.

    virtual void SetCallbacks(const OnNewItem & onNewItem, const OnStart & onStart, const OnStop & onStop) = 0;
    virtual void Start(const std::wstring & ldapRequest, const ConnectionParams & cs) = 0;
    virtual bool IsStarted() const = 0;
    virtual void Stop() = 0;
    virtual void Wait() = 0;
};

}   // namespace adbook