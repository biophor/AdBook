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
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using WpfAdBook.DAL;
using WpfAdBook.Services;
using WpfAdBook.ViewModels;

namespace WpfAdBook
{
    public sealed class ViewModelLocator: IDisposable
    {
        public ViewModelLocator()
        {
            _repository = new AdRepository();
            _settingsService = new SettingsService(autoLoadSettings: true);
            DialogService = new DialogService(_settingsService);
            _dataService = new DataService(_settingsService, _repository);
            _mainWindowViewModel = new MainWindowVM(_dataService, DialogService, _settingsService);
            SettingsWindowViewModel = new SettingsWindowVM(_dataService, DialogService, _settingsService);
            _editPersonWindowViewModel = new EditPersonWindowVM(_dataService, DialogService);
        }

        public ViewModelLocator(ISettingsService settingsService, IRepository repository)
        {
            _repository = repository;
            _settingsService = settingsService;
            DialogService = new DialogService(_settingsService);
            _dataService = new DataService(_settingsService, _repository);
            _mainWindowViewModel = new MainWindowVM(_dataService, DialogService, _settingsService);
            SettingsWindowViewModel = new SettingsWindowVM(_dataService, DialogService, _settingsService);
            _editPersonWindowViewModel = new EditPersonWindowVM(_dataService, DialogService);
        }

        private readonly EditPersonWindowVM _editPersonWindowViewModel;
        private readonly MainWindowVM _mainWindowViewModel;
        private readonly IRepository _repository;
        private readonly IDataService _dataService;
        public IDialogService DialogService { get; }
        private readonly ISettingsService _settingsService;

        public MainWindowVM MainWindowViewModel {
            get => _mainWindowViewModel;
        }

        public SettingsWindowVM SettingsWindowViewModel { get; }

        public EditPersonWindowVM EditPersonWindowViewModel {
            get => _editPersonWindowViewModel;
        }

        private bool disposed = false;

        public void Dispose()
        {
            if (disposed) {
                return;
            }
            _mainWindowViewModel.Dispose();
            _editPersonWindowViewModel.Dispose();
            _dataService.Dispose();
            _repository.Dispose();
            _settingsService.Dispose();
            disposed = true;
        }
    }
}
