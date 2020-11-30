﻿// This is an open source non-commercial project. Dear PVS-Studio, please check it.
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

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WfAdBook
{
    enum CompositeFilterId
    {
        AnyAttribute
    }
    sealed class CompositeFilter: IEquatable<CompositeFilter>
    {
        public CompositeFilterId Id { get; private set; }
        public string DisplayName { get; private set; }
        public CompositeFilter(string displayName, CompositeFilterId id)
        {
            Id = id;
            DisplayName = displayName;
        }
        public override string ToString()
        {
            return DisplayName;
        }

        public bool Equals(CompositeFilter other)
        {
            if (this == other) {
                return true;
            }
            if (other == null) {
                return false;
            }
            if (GetType() != other.GetType()) {
                return false;
            }
            return DisplayName == other.DisplayName && Id == other.Id;
        }
        public override bool Equals(object obj)
        {
            return Equals(obj as CompositeFilter);
        }
        public override int GetHashCode()
        {
            return (int)Id;
        }
    }
}
