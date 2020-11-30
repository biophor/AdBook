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
#include "qmladbookbl_global.h"
#include "ASettingsKeeper.h"
#include "AContactsKeeper.h"
#include "AContactUpdater.h"
#include "ADataExchanger.h"
#include "AContactFinder.h"
#include "ASettingsChecker.h"
#include "AContactPhotoProvider.h"
#include "AClipboardService.h"
#include "AFileSystemService.h"

namespace qmladbook
{
    class QMLADBOOKBL_EXPORT ADependencyResolver
    {
    public:
        virtual ~ADependencyResolver() = 0;

        virtual std::shared_ptr<ASettingsKeeper> GetSettingsKeeper() = 0;
        virtual std::shared_ptr<AContactsKeeper> GetContactsKeeper() = 0;
        virtual std::shared_ptr<adbook::AbstractAdAccessFactory> GetAdFactory() = 0;
        virtual std::shared_ptr<AContactUpdater> GetContactUpdater() = 0;
        virtual std::shared_ptr<ADataExchanger> GetDataExchanger() = 0;
        virtual std::shared_ptr<AContactFinder> GetContactFinder() = 0;
        virtual AContactPhotoProvider * GetContactPhotoProvider() = 0;
        virtual std::shared_ptr<ASettingsChecker> GetSettingsChecker() = 0;
        virtual std::shared_ptr<AClipboardService> GetClipboardService() = 0;
        virtual std::shared_ptr<AFileSystemService> GetFileSystemService() = 0;
    };

    std::shared_ptr< ADependencyResolver> QMLADBOOKBL_EXPORT GetDependencyResolver();
    void QMLADBOOKBL_EXPORT SetDependencyResolver(std::shared_ptr< ADependencyResolver> resolver);
}

