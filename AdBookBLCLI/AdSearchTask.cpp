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
#include "AdSearchTask.h"
#include "AdAccessFactory.h"
namespace adbookcli
{

using namespace System;
using namespace System::Threading::Tasks;

using AdSearchTaskDelegate = System::Action<Object^>;

using SearchStartedProcType = void(__stdcall *)();
using SearchStoppedProcType = void(__stdcall *)();
using PersonFoundProcType = void(__stdcall *)(adbook::AdPersonDesc &&);

using namespace System::Runtime::InteropServices;

void NativeAdSearcherPtr::ReleaseNativeResources()
{
    try {}
    finally{
        adbook::AbstractAdSearcher * ns = reinterpret_cast<adbook::AbstractAdSearcher *>(handle.ToPointer());
        try {
            if (ns != nullptr) {
                if (ns->IsStarted()) {
                    ns->Stop();
                    ns->Wait();
                }
                delete ns;
                ns = nullptr;
            }
            handle = IntPtr::Zero;
        }
        catch (const adbook::Error & e) {
            MY_TRACE(L"%s %s", __FUNCTIONW__, e.What(), e.Where());
        }
        catch (const std::exception & e) {
            MY_TRACE("%s %s", __FUNCTION__, e.what());
        }
    }
}

AdSearchTask::AdSearchTask(adbook::AbstractAdSearcher * searcher, Arguments^ args)
    : Task(
        gcnew AdSearchTaskDelegate(this, &AdSearchTask::TaskProc),
        args,
        cancelTokenSource_->Token,
        System::Threading::Tasks::TaskCreationOptions::LongRunning
        ),
      _nativeSearcher(searcher)
{
    adbook::AbstractAdSearcher::OnStart searchStartedFunction;
    searchStartedFunction = CreateSearchStartedCallback(this);

    adbook::AbstractAdSearcher::OnStop searchStoppedFunction;
    searchStoppedFunction = CreateSearchStoppedCallback(this);

    adbook::AbstractAdSearcher::OnNewItem personFoundFunction;
    personFoundFunction = CreatePersonFoundCallback(this);

    _nativeSearcher->SetCallbacks(personFoundFunction, searchStartedFunction, searchStoppedFunction);
    GC::KeepAlive(this);
}

adbook::AbstractAdSearcher::OnNewItem AdSearchTask::CreatePersonFoundCallback(AdSearchTask ^ searcher)
{
    searcher->_raisePersonFoundDelegate =
        gcnew RaisePersonFoundDelegate(searcher, &AdSearchTask::RaisePersonFoundEvent);

    PersonFoundProcType personFoundCallback = static_cast<PersonFoundProcType>(
        Marshal::GetFunctionPointerForDelegate(searcher->_raisePersonFoundDelegate).ToPointer()
        );
    auto f = [personFoundCallback](adbook::AdPersonDesc && apd) {
        personFoundCallback(std::move(apd));
    };
    return f;
}

adbook::AbstractAdSearcher::OnStart AdSearchTask::CreateSearchStartedCallback(AdSearchTask ^ searcher)
{
    searcher->_raiseSearchStartedDelegate =
        gcnew RaiseSearchStartedDelegate(searcher, &AdSearchTask::RaiseSearchStartedEvent);

    SearchStartedProcType searchStartedCallback = static_cast<SearchStartedProcType>(
        Marshal::GetFunctionPointerForDelegate(searcher->_raiseSearchStartedDelegate).ToPointer()
        );
    auto f = [searchStartedCallback]() {
        searchStartedCallback();
    };
    return f;
}

adbook::AbstractAdSearcher::OnStop AdSearchTask::CreateSearchStoppedCallback(AdSearchTask ^ searcher)
{
    searcher->_raiseSearchStoppedDelegate =
        gcnew RaiseSearchStoppedDelegate(searcher, &AdSearchTask::RaiseSearchStoppedEvent);

    SearchStoppedProcType searchStoppedCallback = static_cast<SearchStoppedProcType>(
        Marshal::GetFunctionPointerForDelegate(searcher->_raiseSearchStoppedDelegate).ToPointer()
        );
    auto f = [searchStoppedCallback]() {
        searchStoppedCallback();
    };
    return f;
}

void AdSearchTask::TaskProc(Object^ arg)
{
    try {
        Arguments^ args = safe_cast<Arguments^>(arg);
        _nativeSearcher->Start(
            args->LdapRequestBuilder->ToUnderlyingType().Get(),
            args->ConnectionParams->ToUnderlyingType()
        );
        _nativeSearcher->Wait();
        GC::KeepAlive(this);
    }
    catch (const adbook::Error & e) {
        MY_TRACE(L"%s %s", __FUNCTIONW__, e.What(), e.Where());
        throw gcnew AdSearchTaskError(gcnew String(e.What()));
    }
    catch (const std::exception & e) {
        MY_TRACE("%s %s", __FUNCTION__, e.what());
        throw gcnew System::Exception(gcnew String(e.what()));
    }

    cancelTokenSource_->Token.ThrowIfCancellationRequested();

}

void AdSearchTask::Cancel()
{
    try {
        cancelTokenSource_->Cancel();
        _nativeSearcher->Stop();
        GC::KeepAlive(this);
    }
    catch (const adbook::Error & e) {
        MY_TRACE(L"%s %s", __FUNCTIONW__, e.What(), e.Where());
        throw gcnew System::Exception(gcnew String(e.What()));
    }
}

void AdSearchTask::RaiseSearchStartedEvent()
{
    SearchStartedEvent(this, System::EventArgs::Empty);
}

void AdSearchTask::RaiseSearchStoppedEvent()
{
    SearchStoppedEvent(this, System::EventArgs::Empty);
}

void AdSearchTask::RaisePersonFoundEvent(adbook::AdPersonDesc && apd)
{
    AdPerson^ madp = gcnew AdPerson(apd);
    PersonFoundEvent(this, gcnew PersonFoundEventArgs(madp));
}

void AdSearchTask::EnsureSearchIsNotRunning()
{
    try {
        _nativeSearcher->Stop();
        _nativeSearcher->Wait();
        GC::KeepAlive(this);
    }
    catch (const adbook::Error & e) {
        MY_TRACE(L"%s %s", __FUNCTIONW__, e.What(), e.Where());
        throw gcnew System::Exception(gcnew String(e.What()));
    }
}

AdSearchTask::~AdSearchTask() {
    this->!AdSearchTask();
}

AdSearchTask::!AdSearchTask() {
    if (nullptr == cancelTokenSource_) {
        delete cancelTokenSource_;
        cancelTokenSource_ = nullptr;
    }
}

}   // namespace adbookcli

