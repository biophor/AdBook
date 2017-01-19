/*
Copyright (C) 2015-2017 Goncharov Andrei.

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

#include "AdPerson.h"
#include "LdapRequest.h"
//#include "AdSearcher.h"
#include "ConnectionParams.h"


namespace adbookcli
{

using namespace System;

public ref class PersonFoundEventArgs : public EventArgs
{
public:
    PersonFoundEventArgs(AdPerson^ person) {
        Person = person;
    }
    property AdPerson^ Person;
};


using System::String;
using System::Collections::Generic::IEnumerable;
using System::Threading::CancellationToken;
using System::Threading::CancellationTokenSource;

public ref class AdSearchTask sealed: public System::Threading::Tasks::Task
{
public:
    ref class Arguments {
    public:
        Arguments(LdapRequest ^ ldapRequest, ConnectionParams ^ connectionParams) {
            LdapRequest = ldapRequest;
            ConnectionParams = connectionParams;
        }
        property LdapRequest ^ LdapRequest;
        property ConnectionParams ^ ConnectionParams;
    };
    AdSearchTask(Arguments^ args);
    ~AdSearchTask();
    !AdSearchTask();

    delegate void PersonFoundDelegate(Object^sender, PersonFoundEventArgs^e);
    delegate void SearchStartedDelegate(Object^sender, EventArgs^ e);
    delegate void SearchStoppedDelegate(Object^sender, EventArgs^ e);

    event PersonFoundDelegate^ PersonFoundEvent;
    event SearchStartedDelegate^ SearchStartedEvent;
    event SearchStoppedDelegate^ SearchStoppedEvent;
        
    void Cancel();
    
private:
    void CleanupUnmanaged();

    void RaisePersonFoundEvent(adbook::AdPersonDesc && apd);
    delegate void RaisePersonFoundDelegate(adbook::AdPersonDesc &&);

    void RaiseSearchStartedEvent();
    delegate void RaiseSearchStartedDelegate();

    void RaiseSearchStoppedEvent();
    delegate void RaiseSearchStoppedDelegate();

    static adbook::AdSearcher::OnNewItem CreatePersonFoundCallback(AdSearchTask ^ searcher);
    static adbook::AdSearcher::OnStart CreateSearchStartedCallback(AdSearchTask ^ searcher);
    static adbook::AdSearcher::OnStop CreateSearchStoppedCallback(AdSearchTask ^ searcher);

    RaisePersonFoundDelegate^ _raisePersonFoundDelegate = nullptr;
    RaiseSearchStartedDelegate^ _raiseSearchStartedDelegate = nullptr;
    RaiseSearchStoppedDelegate^ _raiseSearchStoppedDelegate = nullptr;

private:
    
    void TaskProc(Object^ arg);
    
private:
    CancellationTokenSource^ cancelTokenSource_ = gcnew CancellationTokenSource();
    adbook::AdSearcher * _nativeSearcher;
};

}   // namespace adbookcli

