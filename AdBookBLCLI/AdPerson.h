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
#include "AdAttribute.h"


namespace adbookcli
{
using System::Byte;
using System::Collections::Generic::Dictionary;

public ref class AdPerson sealed
{
public:    
    AdPerson(adbook::AdPersonDesc && adp);
    AdPerson(const adbook::AdPersonDesc & adp);
    AdPerson();
    ~AdPerson();
    !AdPerson();
    bool IsAttributeSet(String ^ ldapAttrName);
    property String ^ StringAttr[String ^ ] {
        String ^ get(String ^ attrName);
        void set(String ^ attrName, String^ value);
    }
    property cli::array<Byte> ^ BinaryAttr[String ^] {
        cli::array<Byte> ^ get(String ^ attrName);
        void set(String ^ attrName, cli::array<Byte> ^ value);
    }    
    AdPerson^ operator = (const adbook::AdPersonDesc & adp);
    AdPerson^ operator = (adbook::AdPersonDesc && adp);

    adbook::AdPersonDesc ToUnderlyingType();
    bool IsAttributeWritable(AttrId id);

    property String ^ Dn {
        String^ get();
    }
private:
	void CleanupUnmanaged();
private:
    // to reduce unnecessary string conversions
    Dictionary<String^, String^> _cashedStringAttributes;
    Dictionary<String^, cli::array<Byte>^> _cashedBinaryAttributes;
    adbook::AdPersonDesc * _personDescPtr;
};

}
