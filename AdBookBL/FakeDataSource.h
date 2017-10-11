/*
Copyright (C) 2015-2020 Goncharov Andrei.

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

#include "AdPersonDesc.h"

#pragma once
namespace adbook
{

// The source of fake data. It's used by FakeConnector and FakeSearcher.
// To use FakeDataSource you should specify the '/FakeDataSource' option as an argument for AdBook.exe
class FakeDataSource
{
public:
    static FakeDataSource & GetDataSource();
    size_t GetPeopleCount() const;
    AdPersonDesc & GetPerson(size_t index);
    AdPersonDesc & GetPersonByDn(const std::wstring & distinguishedName);
    void SetPerson(size_t index, const AdPersonDesc & adp);
    void SetPersonByDn(const AdPersonDesc & adp);
    bool CheckPersonExistance(const std::wstring & distinguishedName);
    void PreExitCleanup();
private:
    FakeDataSource();
    ~FakeDataSource();

private:
    //adbook::Attributes & _attributes;
};

}
