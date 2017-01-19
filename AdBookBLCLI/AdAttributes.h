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
using System::Collections::Generic::IReadOnlyDictionary;
using System::Collections::Generic::IList;
using System::Int32;

public ref class AdAttributes abstract sealed
{
public:
    static property Int32 Count {
        Int32 get() {
            return _attributes->Count;
        }
    }
    static property Int32 TextAttrMaxLength {
        Int32 get() {
            return _textAttrMaxLength;
        }
    }
    static property Int32 BinaryAttrMaxLength {
        Int32 get() {
            return _binaryAttrMaxLength;
        }
    }    
    static property IList<String^>^ LdapAttrNames {
        IList<String^>^ get() {
            return _ldapAttrNames;
        }
    }
    static property IList<AttrId>^ AttrIds {
        IList<AttrId>^ get() {
            return _attrIds;
        }
    }
    static AdAttribute^ Get(AttrId id) {
        return _attributes[id]; //-V108
    }
    static AttrId ConvertAttrId(adbook::Attributes::AttrId id) {
        return static_cast<AttrId>(id);
    }
    static adbook::Attributes::AttrId ConvertAttrId(AttrId id) {
        return static_cast<adbook::Attributes::AttrId>(id);
    }
private:
    static AdAttributes();
private:
    static initonly IReadOnlyDictionary<AttrId, AdAttribute^>^ _attributes;
    static initonly Int32 _textAttrMaxLength;
    static initonly Int32 _binaryAttrMaxLength;
    static initonly IList<String^>^ _ldapAttrNames;
    static initonly IList<AttrId>^ _attrIds;
};

}   // namespace adbookcli

