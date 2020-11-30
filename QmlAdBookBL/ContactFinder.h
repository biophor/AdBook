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

#pragma once

#include "AContactFinder.h"
#include "qmladbookbl_global.h"

namespace qmladbook
{
    class QMLADBOOKBL_EXPORT ContactFinder : public AContactFinder
    {
        Q_OBJECT
    public:
        ContactFinder(
            std::shared_ptr<adbook::AbstractAdAccessFactory> adFactory,
            QObject *parent = nullptr
        );

        virtual ~ContactFinder();

        std::vector<Contact> Peek() override;
        void Start(QString ldapRequest, const ConnectionSettings & settigns) override;
        void Stop() override;
        void EnsureStopped() override;
        bool IsActive() const override;
        void Wait() const override;
        void PropogateLastSearchException();
    private:
        void EnsureAdSearcherInitialized();
        void ContactFoundCallback(adbook::AdPersonDesc && item);
        void SearchStartedCallback();
        void SearchStoppedCallback();
        void SearchErrorCallback(QString errorDescription);

    private:
        std::mutex _contactsMutex;
        std::vector<Contact> _contacts;
        QTime _contactFoundCallbackTime;

        std::shared_ptr<adbook::AbstractAdAccessFactory> _adFactory;
        std::unique_ptr<adbook::AbstractAdSearcher> _adSearcher;
    };
}
