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
using System.ComponentModel;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Security;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using WpfAdBook.Services;

namespace WpfAdBook.ViewModels
{
    public class SettingsWindowVM: INotifyPropertyChanged
    {
        private IDataService _dataService;
        private ISettingsService _settingsService;
        private IDialogService _dialogService;
        public CustomCommand OkCommand { get; internal set; }
        public CustomCommand CancelCommand { get; internal set; }
        public CustomCommand TestCommand { get; internal set; }
        public CustomCommand WindowClosedNotification { get; internal set; }

        public SettingsWindowVM(IDataService dataService, IDialogService dialogService, ISettingsService settingsService)
        {
            _dataService = dataService;
            _dialogService = dialogService;
            _settingsService = settingsService;
            OkCommand = new CustomCommand(ExecuteOkCommand, CanExecuteOkCommand);
            CancelCommand = new CustomCommand(ExecuteCancelCommand, CanExecuteCancelCommand);
            TestCommand = new CustomCommand(ExecuteTestCommand, CanExecuteTestCommand);
            WindowClosedNotification = new CustomCommand(OnWindowClosed, (object obj) => true);

            LoadSettings();
        }

        private void OnWindowClosed(object obj)
        {
        }

        private void LoadSettings()
        {
            Domain = _settingsService.ConnectionParams.Dc;
            ConnectDefaultDomain = _settingsService.ConnectionParams.UseDomainYouAreLoggedIn;
            UseCurrentUserCredentials = _settingsService.ConnectionParams.UseCurrentUserCredentials;
            Login = _settingsService.ConnectionParams.Login;
            SecurePassword = _settingsService.ConnectionParams.Password;
        }

        private bool? _connectDefaultDomain = true;
        public bool? ConnectDefaultDomain {
            get {
                return _connectDefaultDomain;
            }
            set {
                if (_connectDefaultDomain != value) {
                    _connectDefaultDomain = value;
                    RaisePropertyChanged(nameof(ConnectDefaultDomain));
                }
            }
        }

        private bool? _useCurrentUserCredentials = true;
        public bool? UseCurrentUserCredentials {
            get {
                return _useCurrentUserCredentials;
            }
            set {
                if (_useCurrentUserCredentials != value) {
                    _useCurrentUserCredentials = value;
                    RaisePropertyChanged(nameof(UseCurrentUserCredentials));
                }
            }
        }

        private string _domain;
        public string Domain {
            get {
                return _domain;
            }
            set {
                if (_domain != value) {
                    _domain = value;
                    RaisePropertyChanged(nameof(Domain));
                }
            }
        }

        private string _login;
        public string Login {
            get {
                return _login;
            }
            set {
                if (_login != value) {
                    _login = value;
                    RaisePropertyChanged(nameof(Login));
                }
            }
        }

        private SecureString _securePassword;
        public SecureString SecurePassword {
            get {
                return _securePassword;
            }
            set {
                if (_securePassword != value) {
                    _securePassword = value;
                    RaisePropertyChanged(nameof(SecurePassword));
                }
            }
        }

        private bool CanExecuteTestCommand(object arg)
        {
            if (!ConnectDefaultDomain.HasValue || !UseCurrentUserCredentials.HasValue) {
                return false;
            }
            bool connectionParamsTestable = (ConnectDefaultDomain.Value || !string.IsNullOrEmpty(Domain));

            bool credentialParamsTestable = UseCurrentUserCredentials.Value ||
                (!(null == SecurePassword || (0 ==SecurePassword.Length) || string.IsNullOrEmpty(Login)));
            return connectionParamsTestable && credentialParamsTestable;
        }

        private void ExecuteTestCommand(object obj)
        {
            using (var connectionParams = new ConnectionParams()) {
                if (UseCurrentUserCredentials.HasValue) {
                    connectionParams.UseCurrentUserCredentials = UseCurrentUserCredentials.Value;
                }
                if (ConnectDefaultDomain.HasValue) {
                    connectionParams.UseDomainYouAreLoggedIn = ConnectDefaultDomain.Value;
                }
                connectionParams.Dc = Domain;
                connectionParams.Login = Login;
                connectionParams.Password = SecurePassword.Copy();

                using (var wc = new WaitCursor()) {
                    bool dataIsAccessible = _dataService.CheckConnectionParams(connectionParams);
                    wc.Restore();
                    if (dataIsAccessible) {
                        _dialogService.DisplaySuccessMessage(Properties.Resources.TestConnectionSuccessfulMessage);
                    }
                    else {
                        _dialogService.DisplayErrorMessage(Properties.Resources.TestConnectionUnsuccessfulMessage);
                    }
                }
            }
        }

        private bool CanExecuteCancelCommand(object arg)
        {
            return true;
        }

        private void ExecuteCancelCommand(object obj)
        {
            _dialogService.CloseSettingsWindow();
        }

        private bool CanExecuteOkCommand(object arg)
        {
            return true;
        }

        private void ExecuteOkCommand(object obj)
        {
            if (UseCurrentUserCredentials.HasValue) {
                _settingsService.ConnectionParams.UseCurrentUserCredentials = UseCurrentUserCredentials.Value;
            }
            if (ConnectDefaultDomain.HasValue) {
                _settingsService.ConnectionParams.UseDomainYouAreLoggedIn = ConnectDefaultDomain.Value;
            }
            _settingsService.ConnectionParams.Dc = Domain;
            _settingsService.ConnectionParams.Login = Login;
            _settingsService.ConnectionParams.Password = SecurePassword;
            _settingsService.Save();

            _dialogService.CloseSettingsWindow();
        }

        public event PropertyChangedEventHandler PropertyChanged;

        private void RaisePropertyChanged([CallerMemberName]string propertyName="")
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
    }
}
