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
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace WpfAdBook.ViewModels
{

    [Serializable]
    public class FilterRuleVM : IEquatable<FilterRuleVM>, INotifyPropertyChanged
    {
        public FilterRuleVM(FilterVM filter, FilterMatchingRuleVM matchingRule, string value)
        {
            _matchingRule = matchingRule;
            _filter = filter;
            _value = value;
        }

        private FilterMatchingRuleVM _matchingRule;
        public FilterMatchingRuleVM MatchingRule
        {
            get {
                return _matchingRule;
            }
            set {
                _matchingRule = value;
                RaisePropertyChanged(nameof(MatchingRule));
            }
        }

        FilterVM _filter;
        public FilterVM Filter
        {
            get {
                return _filter;
            }
            set {
                if (!_filter.Equals(value)) {
                    _filter = value;
                    RaisePropertyChanged(nameof(Filter));
                }

            }
        }

        string _value;
        public string Value
        {
            get {
                return _value;
            }
            set {
                if (_value != value) {
                    _value = value;
                    RaisePropertyChanged(nameof(Value));
                }
            }
        }

        public event PropertyChangedEventHandler PropertyChanged;

        private void RaisePropertyChanged([CallerMemberName]string propertyName = "")
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }

        public bool Equals(FilterRuleVM other)
        {
            if (ReferenceEquals(other, null)) {
                return false;
            }
            if (ReferenceEquals(this, other)) {
                return true;
            }
            if (GetType() != other.GetType()) {
                return false;
            }
            bool valueEqual = Value.Equals(other.Value);
            bool filterEqual = Filter.Equals(other.Filter);
            bool matchingRule = MatchingRule.Equals(other.MatchingRule);
            return valueEqual && filterEqual && matchingRule;
        }

        public override bool Equals(object obj)
        {
            return Equals(obj as FilterRuleVM);
        }

        public override int GetHashCode()
        {
            return MatchingRule.GetHashCode() ^ Filter.GetHashCode() ^ Value.GetHashCode();
        }
    }
}
