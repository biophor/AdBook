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
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WfAdBook
{
    sealed class FontSettings// IDisposable is not needed
    {
        public Font FilterListFont { get; set; }
        public Font ContactListFont { get; set; }
        public Font ContactDetailsFont { get; set; }

        public FontSettings()
        {
            Load();
        }
        public void Load()
        {
            FilterListFont = Properties.Settings.Default.FilterListFont;
            ContactListFont = Properties.Settings.Default.ContactListFont;
            ContactDetailsFont = Properties.Settings.Default.ContactDetailsFont;
        }
        public void Save()
        {
            if (!Font.Equals(Properties.Settings.Default.FilterListFont, FilterListFont)) {                
                Properties.Settings.Default.FilterListFont = FilterListFont;
            }
            if (!Font.Equals(Properties.Settings.Default.ContactListFont, ContactListFont)) {                
                Properties.Settings.Default.ContactListFont = ContactListFont;
            }
            if (!Font.Equals(Properties.Settings.Default.ContactDetailsFont, ContactDetailsFont)) {                
                Properties.Settings.Default.ContactDetailsFont = ContactDetailsFont;
            }            
        }        
    }
}
