// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
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

using adbookcli;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WpfAdBook.ViewModels
{
    public class PredefinedFilterListVM : IEnumerable<FilterVM>
    {
        static List<FilterVM> _predefinedFilters;

        static PredefinedFilterListVM()
        {
            _predefinedFilters = new List<FilterVM> {
                new AnyAttrsFilterVM(),
                new NameAttrsFilterVM(),
                new PhoneAttrsFilterVM(),
                new SingleAttrFilterVM(AttrId.CommonName),
                new SingleAttrFilterVM(AttrId.Email),
                new SingleAttrFilterVM(AttrId.Title),
                new SingleAttrFilterVM(AttrId.Department),
                new SingleAttrFilterVM(AttrId.DisplayName),
                new SingleAttrFilterVM(AttrId.WorkPhone),
                new SingleAttrFilterVM(AttrId.GivenName),
                new SingleAttrFilterVM(AttrId.SurName),
                new SingleAttrFilterVM(AttrId.Company),
                new SingleAttrFilterVM(AttrId.HomePhone),
                new SingleAttrFilterVM(AttrId.State),
                new SingleAttrFilterVM(AttrId.Locality),
                new SingleAttrFilterVM(AttrId.MobilePhone),
                new SingleAttrFilterVM(AttrId.EmpId)
            };
        }

        public IEnumerator<FilterVM> GetEnumerator()
        {
            return _predefinedFilters.GetEnumerator();
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return _predefinedFilters.GetEnumerator();
        }
    }
}
