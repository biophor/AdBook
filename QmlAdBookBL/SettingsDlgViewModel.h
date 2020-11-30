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
#include <QObject>
#include <QObjectDefs.h>
#include "qmladbookbl_global.h"
#include "Password.h"
#include "ADependencyResolver.h"

namespace qmladbook
{
    class QMLADBOOKBL_EXPORT SettingsDlgViewModel : public QObject
    {
        Q_OBJECT
    public:
        SettingsDlgViewModel();

    public:
        // interface for QML-view
        Q_PROPERTY(bool connectCurrentDomain READ connectCurrentDomain WRITE setConnectCurrentDomain NOTIFY connectCurrentDomainChanged);
        Q_INVOKABLE bool connectCurrentDomain() const;
        Q_INVOKABLE void setConnectCurrentDomain(bool newValue);

        Q_PROPERTY(bool connectAsCurrentUser READ connectAsCurrentUser WRITE setConnectAsCurrentUser NOTIFY connectAsCurrentUserChanged);
        Q_INVOKABLE bool connectAsCurrentUser() const;
        Q_INVOKABLE void setConnectAsCurrentUser(bool newValue);

        Q_PROPERTY(QString address READ address WRITE setAddress NOTIFY addressChanged);
        Q_INVOKABLE QString address() const;
        Q_INVOKABLE void setAddress(QString newValue);

        Q_PROPERTY(QString login READ login WRITE setLogin NOTIFY loginChanged);
        Q_INVOKABLE QString login() const;
        Q_INVOKABLE void setLogin(QString newValue);

        Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY passwordChanged);
        Q_INVOKABLE QString password() const;
        Q_INVOKABLE void setPassword(QString newValue);

        Q_INVOKABLE void save();
        Q_INVOKABLE void load();

        Q_INVOKABLE bool testSettings();

        Q_PROPERTY(QString testSettingsDetails READ testSettingsDetails NOTIFY testSettingsDetailsChanged);
        Q_INVOKABLE QString testSettingsDetails() const;

    signals:
        void connectCurrentDomainChanged();
        void connectAsCurrentUserChanged();
        void addressChanged();
        void loginChanged();
        void passwordChanged();
        void testSettingsDetailsChanged();
    public:
        void SetDependencyResolver(std::shared_ptr< ADependencyResolver> resolver);
    private:
        bool _connectCurrentDomain{ true };
        bool _connectAsCurrentUser{ true };
        QString _address, _login;
        qmladbook::Password _password;
        QString _testSettingsDetails;
        std::shared_ptr< ADependencyResolver> _dependencyResolver;
    };
}
