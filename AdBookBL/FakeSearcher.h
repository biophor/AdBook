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
#include "AbstractAdSearcher.h"

namespace adbook
{

// FakeSearcher is used for searching if you specified the option '/FakeDataSource' for AdBook.exe
// This can be useful if you cant create an AD LDS instance for testing purposes
class FakeSearcher: public AbstractAdSearcher
{
public:

    virtual ~FakeSearcher() override;
    virtual void SetCallbacks(const OnNewItem & onNewItem, const OnStart & onStart, const OnStop & onStop) override;
    virtual void Start(const std::wstring & ldapRequest, const ConnectionParams & cs) override;
    virtual bool IsStarted() const override;
    virtual void Stop() override;
    virtual void Wait() override;
private:
    void ThreadProc();
private:
    OnNewItem onNewItem_;
    OnStart onStart_;
    OnStop onStop_;
    std::future<void> searchResultFuture_;
    std::atomic<bool> stopFlag_;
};

}   // namespace adbook