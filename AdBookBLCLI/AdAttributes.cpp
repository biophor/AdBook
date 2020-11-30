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


#include "Stdafx.h"
#include "AdAttribute.h"
#include "AdAttributes.h"

namespace adbookcli
{
using System::Collections::Generic::Dictionary;
using System::Collections::Generic::List;

void NativeAttributesPtr::ReleaseNativeResources()
{
    try {}
    finally{// preventing memory leaks due to ThreadAbortException
        try {
            adbook::Attributes * attributes = reinterpret_cast<adbook::Attributes *>(handle.ToPointer());
            if (attributes != nullptr) {
                delete attributes;
                handle = IntPtr::Zero;
            }
        }
        catch (const adbook::Error & e) {
            MY_TRACE(L"%s %s", __FUNCTIONW__, e.What(), e.Where());
        }
        catch (const std::exception & e) {
            MY_TRACE("%s %s", __FUNCTION__, e.what());
        }
    }
}


static AdAttributes::AdAttributes()
{
    adbook::Attributes & attributes = adbook::Attributes::GetInstance();
    const auto attrIds = attributes.GetAttrIds();
    Dictionary<AttrId, AdAttribute^>^ dict = gcnew Dictionary<AttrId, AdAttribute^>();
    Dictionary<String^, AdAttribute^>^ dict2 = gcnew Dictionary<String^, AdAttribute^>();
    List<AttrId>^ ids = gcnew List<AttrId>();

    for (const auto attrId : attrIds) {
        AttrId id = ConvertAttrId(attrId);
        ids->Add(id);
        String^ oid = gcnew String(attributes.GetAttrOid(attrId).c_str());
        String^ ldapName = gcnew String(attributes.GetLdapAttrName(attrId).c_str());
        String^ displayName = gcnew String(attributes.GetUiAttrName(attrId).c_str());
        bool isReadOnly = !attributes.IsEditable(attrId);
        bool isString = attributes.IsString(attrId);
        Int32 maxLen = static_cast<Int32>(attributes.GetAttrMaxLength(attrId));
        AdAttribute attr(id, oid, ldapName, displayName, isReadOnly, isString, maxLen);
        dict[id] = %attr;
        dict2[ldapName] = %attr;
    }
    _attrIds = ids;
    _attrMapById = dict;
    _attrMapByName = dict2;
    _textAttrMaxLength = static_cast<Int32>(attributes.GetTextAttrMaxLength());
    _binaryAttrMaxLength = static_cast<Int32>(attributes.GetBinaryAttrMaxLength());

    List<String^>^ names = gcnew List<String^>();
    for (const auto & attrName : attributes.GetLdapAttrNames()) {
        names->Add(gcnew String(attrName.c_str()));
    }
    _ldapAttrNames = names;
}


}
