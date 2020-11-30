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
#include "Contact.h"
#include "ConnectionSettings.h"
#include "qmladbookbl_global.h"

namespace qmladbook
{
    class QMLADBOOKBL_EXPORT AContactFinder : public QObject
    {
        Q_OBJECT

    public:
        AContactFinder(QObject *parent = nullptr);
        virtual ~AContactFinder() = 0;

    signals:
        void contactFound();
        void searchStarted();
        void searchFinished();
        void errorOccurred(QString errorDescription);

    public:
        virtual void PropogateLastSearchException() = 0;
        virtual std::vector<Contact> Peek() = 0;
        virtual void Start(QString ldapRequest, const ConnectionSettings & settigns) = 0;
        virtual void Stop() = 0;
        virtual void EnsureStopped() = 0;
        virtual bool IsActive() const = 0;
        virtual void Wait() const = 0;
    };
}
