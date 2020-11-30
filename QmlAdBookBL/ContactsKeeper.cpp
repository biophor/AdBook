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
#include "ContactsKeeper.h"
#include "ADependencyResolver.h"

namespace qmladbook
{
    ContactsKeeper::ContactsKeeper() = default;
    ContactsKeeper::~ContactsKeeper() = default;

    void ContactsKeeper::Save(const AContactsModel * contactsModel)
    {
        auto dependencyResolver = GetDependencyResolver();
        auto settingsKeeper = dependencyResolver->GetSettingsKeeper();
        auto adAccessFactory = dependencyResolver->GetAdFactory();
        auto adPersonDescKeeper = adAccessFactory->GetAdPersonDescKeeper();
        const adbook::ConnectionParams connectionParams = settingsKeeper->GetConnectionSettings();
        adPersonDescKeeper->SetNameByConnectionParams(connectionParams);

        const int numContacts = contactsModel->rowCount();
        std::vector<adbook::AdPersonDesc> vecAdp;
        vecAdp.reserve(numContacts);

        for (int i = 0; i < numContacts; ++i) {
            vecAdp.push_back(contactsModel->GetContact(i));
        }
        adPersonDescKeeper->Save(vecAdp);
    }

    void ContactsKeeper::Load(AContactsModel * contactsModel)
    {
        auto dependencyResolver = GetDependencyResolver();
        auto settingsKeeper = dependencyResolver->GetSettingsKeeper();
        auto adAccessFactory = dependencyResolver->GetAdFactory();
        auto adPersonDescKeeper = adAccessFactory->GetAdPersonDescKeeper();
        const adbook::ConnectionParams connectionParams = settingsKeeper->GetConnectionSettings();
        adPersonDescKeeper->SetNameByConnectionParams(connectionParams);

        std::vector<adbook::AdPersonDesc> vecAdp;
        adPersonDescKeeper->Load(vecAdp);
        for (auto&& adp : vecAdp) {
            contactsModel->AddContact(Contact(std::move(adp)));
        }
    }
}

