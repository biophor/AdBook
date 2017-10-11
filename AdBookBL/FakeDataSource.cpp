// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
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

#include "stdafx.h"
#include "adsi.h"
#include "error.h"
#include "FakeDataSource.h"

namespace adbook
{
std::once_flag oneTimePopulate;
std::vector<AdPersonDesc> fakePeopleData;

void PopulateFakePeopleData()
{
    AdPersonDesc adp;
    adp.SetStringAttr(L"distinguishedName", L"cn=Stacey Garcia,dc=fake,dc=local");
    adp.SetStringAttr(L"cn", L"Stacey Garcia");
    adp.SetStringAttr(L"mail", L"StaceyDGarcia@fleckens.hu");
    adp.SetStringAttr(L"company", L"EnviroSource Design");
    adp.SetStringAttr(L"title", L"Human resources supervisor");
    adp.SetStringAttr(L"telephoneNumber", L"732-482-7169");
    fakePeopleData.push_back(adp);

    adp.SetStringAttr(L"distinguishedName", L"cn=Robert Foster,dc=fake,dc=local");
    adp.SetStringAttr(L"cn", L"Robert Foster");
    adp.SetStringAttr(L"mail", L"RobertKFoster@rhyta.com");
    adp.SetStringAttr(L"company", L"Erlebacher's");
    adp.SetStringAttr(L"title", L"Coache");
    adp.SetStringAttr(L"telephoneNumber", L"860-781-3399");
    fakePeopleData.push_back(adp);

    adp.SetStringAttr(L"distinguishedName", L"cn=Robert Duckworth,dc=fake,dc=local");
    adp.SetStringAttr(L"cn", L"Robert Duckworth");
    adp.SetStringAttr(L"mail", L"RobertDDuckworth@einrot.com");
    adp.SetStringAttr(L"company", L"Strategic Profit");
    adp.SetStringAttr(L"title", L"Chemical plant and system operator");
    adp.SetStringAttr(L"telephoneNumber", L"989-892-4686");
    fakePeopleData.push_back(adp);

    adp.SetStringAttr(L"distinguishedName", L"cn=Stanley Ellis,dc=fake,dc=local");
    adp.SetStringAttr(L"cn", L"Stanley Ellis");
    adp.SetStringAttr(L"mail", L"StanleyEEllis@dayrep.com");
    adp.SetStringAttr(L"company", L"Mages");
    adp.SetStringAttr(L"title", L"Mechanical engineering technician");
    adp.SetStringAttr(L"telephoneNumber", L"847-374-7727");
    fakePeopleData.push_back(adp);

    adp.SetStringAttr(L"distinguishedName", L"cn=Peter Cabrera,dc=fake,dc=local");
    adp.SetStringAttr(L"cn", L"Peter Cabrera");
    adp.SetStringAttr(L"mail", L"PeterNCabrera@dayrep.com");
    adp.SetStringAttr(L"company", L"Great Western");
    adp.SetStringAttr(L"title", L"Chemical technician");
    adp.SetStringAttr(L"telephoneNumber", L"602-382-5047");
    fakePeopleData.push_back(adp);
}

FakeDataSource::FakeDataSource()
{
    std::call_once(oneTimePopulate, PopulateFakePeopleData);
}

FakeDataSource::~FakeDataSource()
{
    fakePeopleData.clear();
}

FakeDataSource & FakeDataSource::GetDataSource()
{
    static FakeDataSource repository;
    return repository;
}

size_t FakeDataSource::GetPeopleCount() const
{
    return fakePeopleData.size();
}

AdPersonDesc & FakeDataSource::GetPerson(size_t index)
{
    return fakePeopleData[index];
}

AdPersonDesc & FakeDataSource::GetPersonByDn(const std::wstring & distinguishedName)
{
    auto iter = std::find_if(fakePeopleData.begin(), fakePeopleData.end(),
        [&distinguishedName](AdPersonDesc & adp) { return adp.GetDn() == distinguishedName; }
    );
    if (iter == fakePeopleData.cend()) {
        HR_ERROR(E_INVALIDARG);
    }
    return *iter;
}

void FakeDataSource::SetPerson(size_t index, const AdPersonDesc & adp)
{
    fakePeopleData[index] = adp;
}

void FakeDataSource::SetPersonByDn(const AdPersonDesc & adp)
{
    GetPersonByDn(adp.GetDn()) = adp;
}

void FakeDataSource::PreExitCleanup()
{
    fakePeopleData.clear();
}

bool FakeDataSource::CheckPersonExistance(const std::wstring & distinguishedName)
{
    auto iter = std::find_if(fakePeopleData.begin(), fakePeopleData.end(),
        [&distinguishedName](AdPersonDesc & adp) { return adp.GetDn() == distinguishedName; }
    );
    if (iter == fakePeopleData.cend()) {
        return false;
    }
    return true;
}

}   // namespace adbook

