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
#include "AdBookBLExport.h"
#include "ConnectionParams.h"
#include "AbstractAdConnector.h"
#include "AbstractAdSearcher.h"
#include "AbstractAdPersonDescKeeper.h"

namespace adbook
{

class ADBOOKBL_API AbstractAdAccessFactory
{
public:
    virtual std::unique_ptr<AbstractAdConnector> CreateConnector() = 0;
    virtual std::unique_ptr<AbstractAdSearcher> CreateSearcher() = 0;
    virtual std::shared_ptr<AbstractAdPersonDescKeeper> GetAdPersonDescKeeper() = 0;

    virtual AbstractAdConnector * CreateConnectorRawPtr() = 0;
    virtual AbstractAdSearcher * CreateSearcherRawPtr() = 0;
    virtual AbstractAdPersonDescKeeper * GetAdPersonDescKeeperRawPtr() = 0;

    virtual ~AbstractAdAccessFactory() = 0;
};

ADBOOKBL_API std::shared_ptr<AbstractAdAccessFactory> GetAdAccessFactory();

ADBOOKBL_API AbstractAdAccessFactory * CreateAdAccessFactoryRawPtr();

ADBOOKBL_API void SetAdAccessFactory (
    std::shared_ptr<AbstractAdAccessFactory> factory
);


}   // namespace adbook

