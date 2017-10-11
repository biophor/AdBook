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
using System.Collections;
using System.ComponentModel;
using System.Runtime.CompilerServices;

namespace WpfAdBook.ViewModels
{
    [Serializable]
    public class FilterMatchingRuleVM: IEquatable<FilterMatchingRuleVM>
    {
        public string DisplayName { get; internal set; }
        public LdapRequestBuilder.MatchingRule MatchingRule { get; internal set; }

        public FilterMatchingRuleVM(LdapRequestBuilder.MatchingRule matchingRule)
        {
            MatchingRule = matchingRule;
            DisplayName = GetDisplayName(matchingRule);
        }

        private string GetDisplayName(LdapRequestBuilder.MatchingRule matchingRule)
        {
            switch (matchingRule) {
                case LdapRequestBuilder.MatchingRule.BeginWith:
                    return Properties.Resources.MatchingRule_BeginsWith;
                case LdapRequestBuilder.MatchingRule.Contains:
                    return Properties.Resources.MatchingRule_Contains;
                case LdapRequestBuilder.MatchingRule.EndWith:
                    return Properties.Resources.MatchingRule_EndsWith;
                case LdapRequestBuilder.MatchingRule.ExactMatch:
                    return Properties.Resources.MatchingRule_ExactMatch;
                default:
                    throw new InvalidEnumArgumentException();
            }
        }

        public bool Equals(FilterMatchingRuleVM other)
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
            return (MatchingRule == other.MatchingRule);
        }

        public override bool Equals(object obj)
        {
            return Equals(obj as FilterMatchingRuleVM);
        }

        public override int GetHashCode()
        {
            return MatchingRule.GetHashCode();
        }

        public override string ToString()
        {
            return DisplayName;
        }
    }

    [Serializable]
    public class PredefinedMatchingRulesVM: IEnumerable<FilterMatchingRuleVM>
    {
        [NonSerialized]
        private static readonly List<FilterMatchingRuleVM> _predefinedRules = new List<FilterMatchingRuleVM>() {
                new FilterMatchingRuleVM(LdapRequestBuilder.MatchingRule.BeginWith),
                new FilterMatchingRuleVM(LdapRequestBuilder.MatchingRule.Contains),
                new FilterMatchingRuleVM(LdapRequestBuilder.MatchingRule.EndWith),
                new FilterMatchingRuleVM(LdapRequestBuilder.MatchingRule.ExactMatch)
        };

        public IEnumerator<FilterMatchingRuleVM> GetEnumerator()
        {
            return _predefinedRules.GetEnumerator();
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return _predefinedRules.GetEnumerator();
        }
    }

    public interface IVisitor
    {
        void Visit(LdapRequestBuilder ldapReq, SingleAttrFilterVM af, FilterMatchingRuleVM matchingRule, string value);
        void Visit(LdapRequestBuilder ldapReq, AnyAttrsFilterVM af, FilterMatchingRuleVM matchingRule, string value);
    }

    [Serializable]
    internal class Visitor : IVisitor
    {
        public void Visit(LdapRequestBuilder ldapReq, AnyAttrsFilterVM af, FilterMatchingRuleVM matchingRule, string value)
        {
            foreach (var id in adbookcli.AdAttributes.AttrIds) {
                var attr = adbookcli.AdAttributes.Get(id);
                if (!attr.IsString) {
                    continue;
                }
                Visit(ldapReq, new SingleAttrFilterVM(id), matchingRule, value);
            }
            ldapReq.AddOR();
        }

        public void Visit(LdapRequestBuilder ldapReq, SingleAttrFilterVM af, FilterMatchingRuleVM matchingRule, string value)
        {
            ldapReq.AddRule(af.AttributeId, matchingRule.MatchingRule, value);
        }
    }

    [Serializable]
    public abstract class FilterVM
    {
        public abstract string DisplayName { get; }
        public abstract void Accept(IVisitor visitor, LdapRequestBuilder ldapReq, FilterMatchingRuleVM matchingRule, string value);
    }

    [Serializable]
    public class SingleAttrFilterVM: FilterVM, IEquatable<SingleAttrFilterVM>
    {
        public AttrId AttributeId { get; set; }

        public SingleAttrFilterVM(AttrId attrId)
        {
            AttributeId = attrId;
        }

        public override string DisplayName {
            get {
                return adbookcli.AdAttributes.Get(AttributeId).DisplayName;
            }
        }

        public override void Accept(IVisitor visitor, LdapRequestBuilder ldapReq, FilterMatchingRuleVM matchingRule, string value)
        {
            visitor.Visit(ldapReq, this, matchingRule, value);
        }

        public bool Equals(SingleAttrFilterVM other)
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
            return AttributeId == other.AttributeId;
        }

        public override bool Equals(object obj)
        {
            return Equals(obj as SingleAttrFilterVM);
        }

        public override int GetHashCode()
        {
            return AttributeId.GetHashCode();
        }

        public override string ToString()
        {
            return DisplayName;
        }
    }

    [Serializable]
    public class AnyAttrsFilterVM: FilterVM, IEquatable<AnyAttrsFilterVM>
    {
        public override string DisplayName {
            get {
                return Properties.Resources.Filter_AnyAttribute;
            }
        }

        public override void Accept(IVisitor visitor, LdapRequestBuilder ldapReq, FilterMatchingRuleVM matchingRule, string value)
        {
            visitor.Visit(ldapReq, this, matchingRule, value);
        }

        public bool Equals(AnyAttrsFilterVM other)
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
            return DisplayName == other.DisplayName;
        }

        public override bool Equals(object obj)
        {
            return Equals(obj as AnyAttrsFilterVM);
        }

        public override int GetHashCode()
        {
            return DisplayName.GetHashCode();
        }

        public override string ToString()
        {
            return DisplayName;
        }
    }

    public class PredefinedFilterListVM : IEnumerable<FilterVM>
    {
        static List<FilterVM> _predefinedFilters;

        static PredefinedFilterListVM()
        {
            _predefinedFilters = new List<FilterVM> {
                new AnyAttrsFilterVM(),
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

    [Serializable]
    public class FilterRuleVM: IEquatable<FilterRuleVM>, INotifyPropertyChanged
    {
        public FilterRuleVM(FilterVM filter, FilterMatchingRuleVM matchingRule, string value)
        {
            _matchingRule = matchingRule;
            _filter = filter;
            _value = value;
        }

        private FilterMatchingRuleVM _matchingRule;
        public FilterMatchingRuleVM MatchingRule {
            get {
                return _matchingRule;
            }
            set {
                _matchingRule = value;
                RaisePropertyChanged(nameof(MatchingRule));
            }
        }

        FilterVM _filter;
        public FilterVM Filter {
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
        public string Value {
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

