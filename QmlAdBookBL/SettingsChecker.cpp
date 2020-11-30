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
#include "SettingsChecker.h"
#include "ADependencyResolver.h"

namespace qmladbook
{
    SettingsChecker::SettingsChecker(std::shared_ptr<adbook::AbstractAdAccessFactory> _adFactory)
        : _adFactory{ _adFactory }
    {
    }

    ASettingsChecker::CheckResult SettingsChecker::Check(const ConnectionSettings & settings)
    {
        auto connector = _adFactory->CreateConnector();
        adbook::ConnectionParams connectionParams = settings;
        ASettingsChecker::CheckResult checkResult;
        try {
            connector->Connect(connectionParams);
            checkResult.status = CheckResultStatus::Ok;
            checkResult.details.clear();
        }
        catch (const adbook::HrError & e)
        {
            checkResult.status = CheckResultStatus::FailedToConnect;
            checkResult.details = QString::fromStdWString(e.What());
        }
        catch (const std::exception & e)
        {
            checkResult.status = CheckResultStatus::FailedToConnect;
            checkResult.details = QString::fromLatin1(e.what());
        }
        return checkResult;
    }
}

