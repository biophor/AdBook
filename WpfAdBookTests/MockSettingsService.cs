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
using adbookcli;
using WpfAdBook.ViewModels;
using System.Collections.ObjectModel;
using System.Drawing;
using System.Diagnostics;

namespace WpfAdBook.Services
{
    public class MockSettingsService : ISettingsService, IDisposable
    {
        public ConnectionParams ConnectionParams { get; } = new ConnectionParams();
        
        public FilteringParams FilteringParams { get; } = new FilteringParams();

        public ObservableCollection<AdPersonVM> LocallyKeptPersons { get; set; } = new ObservableCollection<AdPersonVM>();

        public Size MainWndSize { get; set; }
        public Point MainWndPos { get; set; }

        public bool MainWndMaximized { get; set; }

        public void Save()
        {
            Debug.WriteLine("MockSettingsService.Save()");            
        }

        public void Load()
        {
            Debug.WriteLine("MockSettingsService.Load()");            
        }

        public void Dispose()
        {
            ConnectionParams.Dispose();
        }
    }
}
