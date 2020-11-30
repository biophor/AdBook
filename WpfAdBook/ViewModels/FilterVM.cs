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
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Collections;
using System.ComponentModel;
using System.Runtime.CompilerServices;

namespace WpfAdBook.ViewModels
{
    [Serializable]
    public abstract class FilterVM
    {
        public abstract string DisplayName { get; }
        public abstract void Accept(IVisitor visitor, LdapRequestBuilder ldapReq, FilterMatchingRuleVM matchingRule, string value);
    }

    public interface IVisitor
    {
        void Visit(LdapRequestBuilder ldapReq, SingleAttrFilterVM af, FilterMatchingRuleVM matchingRule, string value);
        void Visit(LdapRequestBuilder ldapReq, AnyAttrsFilterVM af, FilterMatchingRuleVM matchingRule, string value);
        void Visit(LdapRequestBuilder ldapReq, NameAttrsFilterVM af, FilterMatchingRuleVM matchingRule, string value);
        void Visit(LdapRequestBuilder ldapReq, PhoneAttrsFilterVM af, FilterMatchingRuleVM matchingRule, string value);
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

        public void Visit(LdapRequestBuilder ldapReq, NameAttrsFilterVM af, FilterMatchingRuleVM matchingRule, string value)
        {
            ldapReq.AddRule(adbookcli.AttrId.CommonName, matchingRule.MatchingRule, value);
            ldapReq.AddRule(adbookcli.AttrId.DisplayName, matchingRule.MatchingRule, value);
            ldapReq.AddRule(adbookcli.AttrId.GivenName, matchingRule.MatchingRule, value);
            ldapReq.AddRule(adbookcli.AttrId.SurName, matchingRule.MatchingRule, value);
            ldapReq.AddOR();
        }

        public void Visit(LdapRequestBuilder ldapReq, PhoneAttrsFilterVM af, FilterMatchingRuleVM matchingRule, string value)
        {
            ldapReq.AddRule(adbookcli.AttrId.HomePhone, matchingRule.MatchingRule, value);
            ldapReq.AddRule(adbookcli.AttrId.MobilePhone, matchingRule.MatchingRule, value);
            ldapReq.AddRule(adbookcli.AttrId.WorkPhone, matchingRule.MatchingRule, value);
            ldapReq.AddOR();
        }
    }
}

