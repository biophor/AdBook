/*
Copyright (C) 2015-2017 Goncharov Andrei.

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
#include "AdPerson.h"

namespace adbookcli
{
using namespace System;
using namespace System::Collections::Generic;


public ref class AdPersonListKeeper sealed: IEnumerator<AdPerson^>, IEnumerable<AdPerson^>,
                                     Collections::IEnumerable, Collections::IEnumerator
{
public:
    void Add(AdPerson^ adp);
    void Save();
    void Load();
    AdPersonListKeeper();
    ~AdPersonListKeeper();
    !AdPersonListKeeper();

    virtual bool MoveNext();
    virtual void Reset();
    virtual property adbookcli::AdPerson ^ Current {
        adbookcli::AdPerson ^ get();
    }
    virtual IEnumerator<adbookcli::AdPerson ^> ^ GetEnumerator();
private:
	void CleanupUnmanaged();
private:
    Int32 _itemIndex = -1;
    adbook::AdPersonDescList * _adpList;

private:
    virtual property System::Object ^ IEnumerator_Current {
        virtual System::Object^ get() sealed = Collections::IEnumerator::Current::get;
    }
    virtual Collections::IEnumerator ^ IEnumerable_GetEnumerator() sealed = 
        Collections::IEnumerable::GetEnumerator;    
};

}   // namespace adbookcli
