﻿// This is an open source non-commercial project. Dear PVS-Studio, please check it.
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
using System.IO;
using System.Linq;
using System.Security;
using System.Text;
using System.Threading.Tasks;
using adbookcli;
using System.Collections.ObjectModel;
using WpfAdBook.ViewModels;

namespace WpfAdBook.Services
{
    public class FilteringParams
    {
        public string SerializedFilterRules { get; set; }
        public bool StrictFiltering { get; set; } = true;
    }

    public interface ISettingsService: IDisposable
    {
        ConnectionParams ConnectionParams { get; set; }
        FilteringParams FilteringParams { get; set; }
        string SerializedFilterValues { get; set; }
        ObservableCollection<AdPersonVM> SavedSearchResult { get; set; }

        System.Drawing.Size MainWndSize { get; set; }
        System.Drawing.Point MainWndPos { get; set; }
        bool MainWndMaximized { get; set; }

        System.Drawing.Size EditPersonWndSize { get; set; }
        System.Drawing.Point EditPersonWndPos { get; set; }

        void Save();
        void Load();
    }
}

