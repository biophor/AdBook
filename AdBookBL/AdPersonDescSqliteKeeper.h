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

#include "AbstractAdPersonDescKeeper.h"

namespace adbook
{
 // It is used to keep the local copy of search results
class AdPersonDescSqliteKeeper : public AbstractAdPersonDescKeeper
{
public:
    AdPersonDescSqliteKeeper(bool fakeData);

    // set the filename where data is saved.
    void SetNameByConnectionParams (
        const ConnectionParams & connectionParams
    ) override;

    void Load (
        std::vector<AdPersonDesc> &
    ) override;

    void Save (
        const std::vector<AdPersonDesc> &
    ) override;

private:
    static std::wstring GetDbFileNameSuffix (
        const ConnectionParams & connectionParams
    );

    std::wstring GetSqliteDbFilePath (
        const ConnectionParams & connectionParams
    );

    std::wstring GetSqliteDbFileName (
        const ConnectionParams & connectionParams
    );

    void InternalLoad (
        std::vector<AdPersonDesc> & people
    );

    void InternalSave (
        const std::vector<AdPersonDesc> & people
    );

private:
    bool _fakeData = false;
    ConnectionParams _connectionParams;
};

}
