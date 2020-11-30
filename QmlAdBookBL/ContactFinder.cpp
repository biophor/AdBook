/*
Copyright (C) 2015-2021 Andrei Goncharov.

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
#include "ContactFinder.h"

namespace qmladbook
{

    ContactFinder::ContactFinder (
        std::shared_ptr<adbook::AbstractAdAccessFactory> adFactory,
        QObject *parent
    ) : AContactFinder(parent), _adFactory{ adFactory }
    {
    }

    ContactFinder::~ContactFinder()
    {
    }

    std::vector<Contact> ContactFinder::Peek()
    {
        std::scoped_lock locker{ _contactsMutex };
        std::vector<Contact> ret = std::move(_contacts);
        return ret;
    }

    void ContactFinder::EnsureAdSearcherInitialized()
    {
        if (!_adSearcher) {
            _adSearcher = _adFactory->CreateSearcher();
        }
        _adSearcher->SetCallbacks(
            [this](adbook::AdPersonDesc && arg) { ContactFoundCallback(std::move(arg)); },
            [this]() { SearchStartedCallback(); },
            [this]() { SearchStoppedCallback(); },
            [this](const std::wstring & errorDesc) { SearchErrorCallback(QString::fromStdWString(errorDesc)); }
        );
    }

    void ContactFinder::ContactFoundCallback(adbook::AdPersonDesc && adp)
    {
        {
            std::scoped_lock locker(_contactsMutex);
            _contacts.emplace_back(std::move(adp));
        }

        const QTime currentTime = QTime::currentTime();
        if (_contactFoundCallbackTime.msecsTo(currentTime) > 500)
        {
            emit contactFound();
            _contactFoundCallbackTime = currentTime;
        }
    }

    void ContactFinder::SearchStartedCallback()
    {
        _contactFoundCallbackTime = QTime::currentTime();
        emit searchStarted();
    }

    void ContactFinder::SearchStoppedCallback()
    {
        bool emitContactFound = false;
        {
            std::scoped_lock locker(_contactsMutex);
            if (!_contacts.empty()) {
                emitContactFound = true;
            }
        }
        if (emitContactFound) {
            emit contactFound();
        }
        emit searchFinished();
    }

    void ContactFinder::SearchErrorCallback(QString errorDescription)
    {
        emit errorOccurred(errorDescription);
    }

    void ContactFinder::Start(QString ldapRequest, const ConnectionSettings & settings)
    {
        if (IsActive()) {
            throw adbook::HrError(E_UNEXPECTED, L"Start called when search is active", __FUNCTIONW__);
        }
        EnsureAdSearcherInitialized();

        // try to connect before searching
        auto connector = _adFactory->CreateConnector();
        // if the server cannot be contacted the Connect method will throws an exception
        connector->Connect(settings);

        _adSearcher->Start(ldapRequest.toStdWString(), settings);
    }

    void ContactFinder::Stop()
    {
        if (IsActive()) {
            _adSearcher->Stop();
        }
    }

    void ContactFinder::EnsureStopped()
    {
        if (_adSearcher && _adSearcher->IsStarted()) {
            _adSearcher->Stop();
            _adSearcher->Wait();
        }
    }

    bool ContactFinder::IsActive() const
    {
        return _adSearcher && _adSearcher->IsStarted();
    }

    void ContactFinder::PropogateLastSearchException()
    {
        if (_adSearcher) {
            _adSearcher->PropogateSearchException();
        }
    }

    void ContactFinder::Wait() const
    {
        if (_adSearcher && _adSearcher->IsStarted()) {
            _adSearcher->Wait();
        }
    }
}

