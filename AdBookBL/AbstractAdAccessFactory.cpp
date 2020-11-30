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
#include "AdSearcher.h"
#include "AdConnector.h"
#include "Attributes.h"
#include "AdPersonDescSqliteKeeper.h"
#include "AbstractAdAccessFactory.h"

namespace adbook
{

AbstractAdAccessFactory::~AbstractAdAccessFactory() = default;

class RealFactory : public AbstractAdAccessFactory
{
public:
    RealFactory() {
        bool fakeData = false;
        _adPersonDescKeeper = std::make_shared<AdPersonDescSqliteKeeper>(fakeData);
    }
    std::unique_ptr<AbstractAdConnector> CreateConnector() override {
        return std::make_unique<AdConnector>();
    }
    std::unique_ptr<AbstractAdSearcher> CreateSearcher() override {
        return std::make_unique<AdSearcher>();
    }
    std::shared_ptr<AbstractAdPersonDescKeeper> GetAdPersonDescKeeper() override {
        return _adPersonDescKeeper;
    }

    AbstractAdConnector * CreateConnectorRawPtr() override {
        return new AdConnector();
    }
    AbstractAdSearcher * CreateSearcherRawPtr() override {
        return new AdSearcher();
    }

    AbstractAdPersonDescKeeper * GetAdPersonDescKeeperRawPtr() override {
        static AdPersonDescSqliteKeeper sqliteKeeper(false);
        return &sqliteKeeper;
    }

    virtual ~RealFactory() = default;

    RealFactory(const RealFactory & factory) = delete;
    RealFactory(RealFactory && factory) = delete;

    RealFactory & operator = (const RealFactory & factory) = delete;
    RealFactory & operator = (RealFactory && factory) = delete;
private:
    std::shared_ptr<AdPersonDescSqliteKeeper> _adPersonDescKeeper;
};

std::shared_ptr<AbstractAdAccessFactory> globalFactory;

std::shared_ptr<AbstractAdAccessFactory> GetAdAccessFactory()
{
    if (!globalFactory) {
        globalFactory = std::make_shared< RealFactory>();
    }
    return globalFactory;
}

AbstractAdAccessFactory * CreateAdAccessFactoryRawPtr()
{
    return new RealFactory();
}

void SetAdAccessFactory( std::shared_ptr<AbstractAdAccessFactory> factory )
{
    globalFactory = factory;
}

}   // namespace adbook

