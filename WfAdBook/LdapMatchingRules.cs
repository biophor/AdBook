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
using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WfAdBook
{
    class LdapMatchingRules: IEnumerable<LdapMatchingRule>
    {
        public static LdapMatchingRules Instance { get; } = new LdapMatchingRules();

        public IEnumerator<LdapMatchingRule> GetEnumerator()
        {
            foreach (var r in _rules) {
                yield return r;
            }
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return GetEnumerator();
        }

        private LdapMatchingRule[] _rules = new LdapMatchingRule[] {
            new LdapMatchingRule(
                Properties.Resources.LdapMatchingRuleContainsDisplayName,
                adbookcli.LdapRequestBuilder.MatchingRule.Contains
                ),
            new LdapMatchingRule(
                Properties.Resources.LdapMatchingRuleBeginsWithDisplayName,
                adbookcli.LdapRequestBuilder.MatchingRule.BeginWith
                ),
            new LdapMatchingRule(
                Properties.Resources.LdapMatchingRuleEndsWithDisplayName,
                adbookcli.LdapRequestBuilder.MatchingRule.EndWith
                ),
            new LdapMatchingRule(
                Properties.Resources.LdapMatchingRuleExactMatchDisplayName,
                adbookcli.LdapRequestBuilder.MatchingRule.ExactMatch
                )
        };
    }
}
