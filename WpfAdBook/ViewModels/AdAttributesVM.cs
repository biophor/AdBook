// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
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

using adbookcli;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WpfAdBook.ViewModels
{
    public class AdAttributesVM
    {
        public static AdAttribute CommonName { get; }
        public static AdAttribute Email { get; }
        public static AdAttribute Company { get; }
        public static AdAttribute Title { get; }
        public static AdAttribute Department { get; }
        public static AdAttribute State { get; }
        public static AdAttribute EmpId { get; }
        public static AdAttribute SurName { get; }
        public static AdAttribute GivenName { get; }
        public static AdAttribute DisplayName { get; }
        public static AdAttribute WorkPhone { get; }
        public static AdAttribute HomePhone { get; }
        public static AdAttribute MobilePhone { get; }
        public static AdAttribute Locality { get; }
        public static AdAttribute ThumbnailPhoto { get; }

        static AdAttributesVM()
        {
            CommonName = AdAttributes.Get(AttrId.CommonName);
            Company = AdAttributes.Get(AttrId.Company);
            Title = AdAttributes.Get(AttrId.Title);
            Department = AdAttributes.Get(AttrId.Department);
            State = AdAttributes.Get(AttrId.State);
            Email = AdAttributes.Get(AttrId.Email);
            SurName = AdAttributes.Get(AttrId.SurName);
            EmpId = AdAttributes.Get(AttrId.EmpId);
            GivenName = AdAttributes.Get(AttrId.GivenName);
            DisplayName = AdAttributes.Get(AttrId.DisplayName);
            WorkPhone = AdAttributes.Get(AttrId.WorkPhone);
            HomePhone = AdAttributes.Get(AttrId.HomePhone);
            MobilePhone = AdAttributes.Get(AttrId.MobilePhone);
            Locality = AdAttributes.Get(AttrId.Locality);
            ThumbnailPhoto = AdAttributes.Get(AttrId.ThumbnailPhoto);
        }
    }
}
