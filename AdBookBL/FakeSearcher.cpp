// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/*
Copyright (C) 2015-2020 Andrei Goncharov.

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

#include "stdafx.h"
#include "FakeSearcher.h"
#include "FakeDataSource.h"
#include "ConnectionParams.h"

namespace adbook
{

FakeSearcher::~FakeSearcher()
{

}

void FakeSearcher::SetCallbacks(const OnNewItem & onNewItem, const OnStart & onStart, const OnStop & onStop)
{
    onNewItem_ = onNewItem;
    onStart_ = onStart;
    onStop_ = onStop;
}

void FakeSearcher::Start(const std::wstring & /*ldapRequest*/, const ConnectionParams & /*cs*/)
{
    stopFlag_ = false;
    searchResultFuture_ = std::async(std::launch::async, &FakeSearcher::ThreadProc, this);
}

void FakeSearcher::ThreadProc()
{
    if (onStart_) {
        onStart_();
    }
    auto & dataSource = FakeDataSource::GetDataSource();

    for (size_t i = 0, numPeople = dataSource.GetPeopleCount(); i < numPeople; ++i) {
        if (stopFlag_) {
            break;
        }
        AdPersonDesc adp = dataSource.GetPerson(i);
        onNewItem_(std::move(adp));
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    if (onStop_) {
        onStop_();
    }
}

bool FakeSearcher::IsStarted() const
{
    if (searchResultFuture_.valid())
    {
        return (searchResultFuture_.wait_for(std::chrono::seconds(0)) == std::future_status::timeout);
    }
    return false;
}

void FakeSearcher::Stop()
{
    stopFlag_ = true;
}

void FakeSearcher::Wait()
{
    searchResultFuture_.wait();
}

void FakeSearcher::PropogateSearchException()
{
    searchResultFuture_.get();
}

}   // namespace adbook


