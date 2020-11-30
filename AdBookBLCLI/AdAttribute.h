/*
Copyright (C) 2015-2017 Andrei Goncharov.

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

namespace adbookcli
{

using System::String;
using System::Int32;

public enum class AttrId {
    CommonName = adbook::Attributes::CommonName,
    Email = adbook::Attributes::Email,
    Company = adbook::Attributes::Company,
    Title = adbook::Attributes::Title,
    Department = adbook::Attributes::Department,
    State = adbook::Attributes::State,
    EmpId = adbook::Attributes::EmpId,
    SurName = adbook::Attributes::SurName,
    GivenName = adbook::Attributes::GivenName,
    DisplayName = adbook::Attributes::DisplayName,
    WorkPhone = adbook::Attributes::WorkPhone,
    HomePhone = adbook::Attributes::HomePhone,
    MobilePhone = adbook::Attributes::MobilePhone,
    Locality = adbook::Attributes::Locality,
    ThumbnailPhoto = adbook::Attributes::ThumbnailPhoto,
    Dn = adbook::Attributes::Dn,
    AttrIdEnd = adbook::Attributes::AttrIdEnd
};

public ref class AdAttribute: System::IEquatable<AdAttribute^>
{
public:
    AdAttribute(AttrId id, String^ oid, String^ ldapName, String^ displayName, 
        bool isReadOnly, bool isString,
        Int32 maxLen
    )
        : _id(id), _oid(oid), _ldapName(ldapName), _displayName(displayName), 
            _isReadOnly(isReadOnly), _isString(isString), _maxLen(maxLen)
    {}
    property AttrId Id {
        AttrId get() {
            return _id;
        }
    }    
    property System::String^ Oid {
        System::String^ get() {
            return _oid;
        }
    }
    property System::String^ LdapName {
        System::String^ get() {
            return _ldapName;
        }
    }
    property System::String^ DisplayName {
        System::String^ get() {
            return _displayName;
        }
    }
    property bool IsString {
        bool get() {
            return _isString;
        }
    }
    property bool IsReadOnly {
        bool get() {
            return _isReadOnly;
        }
    }
    property Int32 MaxLen {
        Int32 get() {
            return _maxLen;
        }
    }
    // Object::Equals override
    String^ ToString() override {
        return DisplayName;
    }
    Int32 GetHashCode() override {
        return static_cast<Int32>(_id);
    }    
    bool Equals(Object^ o) override {
        return Equals(dynamic_cast<AdAttribute^>(o));
    }
    // System::IEquatable implementation
    virtual bool Equals(AdAttribute ^ other);
private:
    initonly AttrId _id;
    System::String ^ _ldapName, ^ _displayName, ^ _oid;
    initonly bool _isString, _isReadOnly;
    initonly Int32 _maxLen;  // chars for text, bytes for binary attrs
};

}   // namespace adbookcli

