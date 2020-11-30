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

using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

namespace WpfAdBook.Services
{
    public class DialogService: IDialogService
    {
        private AboutWindow _aboutWindow;
        private SettingsWindow _settingsWindow;
        private EditPersonWindow _editPersonWindow;
        ISettingsService _settingService;

        public DialogService(ISettingsService settingService)
        {
            _settingService = settingService;
        }

        public bool RequestConfirmationOnAppExit()
        {
            var messageBoxResult = MessageBox.Show(
                Properties.Resources.AppExitConfirmationRequst,
                Properties.Resources.AppTitleForMessageBox,
                MessageBoxButton.OKCancel, MessageBoxImage.Information
            );
            if (messageBoxResult == MessageBoxResult.OK) {
                return true;
            }
            return false;
        }

        public bool IsSettingsWindowVisible()
        {
            return _settingsWindow != null && _settingsWindow.IsVisible;
        }

        public void DisplaySettingsWindow()
        {
            _settingsWindow = new SettingsWindow();
            _settingsWindow.ShowDialog();
        }
        public void CloseSettingsWindow()
        {
            if (_settingsWindow != null) {
                _settingsWindow.Close();
                _settingsWindow = null;
            }
        }

        public bool IsPersonEditorVisible()
        {
            return _editPersonWindow != null && _editPersonWindow.IsVisible;
        }

        public void DisplayEditPersonWindow()
        {
            _editPersonWindow = new EditPersonWindow();
            _editPersonWindow.Owner = Application.Current.MainWindow;

            RestoreEditPersonWindowState();

            _editPersonWindow.ShowDialog();
        }

        public void CloseEditPersonWindow()
        {
            if (_editPersonWindow != null) {
                _editPersonWindow.Close();
                _editPersonWindow = null;
            }
        }

        public void SaveEditPersonWindowState()
        {
            _settingService.EditPersonWndSize = new System.Drawing.Size(
                    (int)_editPersonWindow.Width, (int)_editPersonWindow.Height
                    );

            _settingService.EditPersonWndPos = new System.Drawing.Point(
                (int)_editPersonWindow.Left, (int)_editPersonWindow.Top
                );
        }

        public void RestoreEditPersonWindowState()
        {
            _editPersonWindow.Width = _settingService.EditPersonWndSize.Width;
            _editPersonWindow.Height = _settingService.EditPersonWndSize.Height;

            _editPersonWindow.Left = _settingService.EditPersonWndPos.X;
            _editPersonWindow.Top = _settingService.EditPersonWndPos.Y;
        }

        public string SelectPhoto()
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();
            openFileDialog.Multiselect = false;
            openFileDialog.Title = Properties.Resources.SelectPhotoDialogCaption;
            openFileDialog.Filter = "JPEG files|*.jpg;*.jpeg";
            if (openFileDialog.ShowDialog() == true) {
                return openFileDialog.FileName;
            }
            return "";
        }

        public void CloseMainWindow()
        {
            Application.Current.MainWindow.Close();
        }

        public void SaveMainWindowState()
        {
            var mainWnd = Application.Current.MainWindow;
            _settingService.MainWndSize = new System.Drawing.Size((int)mainWnd.Width, (int)mainWnd.Height);
            _settingService.MainWndPos = new System.Drawing.Point((int)mainWnd.Left, (int)mainWnd.Top);
            _settingService.MainWndMaximized = (mainWnd.WindowState & WindowState.Maximized) != 0;
        }

        public void RestoreMainWindowState()
        {
            var mainWnd = Application.Current.MainWindow;
            mainWnd.Width = _settingService.MainWndSize.Width;
            mainWnd.Height = _settingService.MainWndSize.Height;
            mainWnd.Left = _settingService.MainWndPos.X;
            mainWnd.Top = _settingService.MainWndPos.Y;
            if (_settingService.MainWndMaximized) {
                mainWnd.WindowState |= WindowState.Maximized;
            }
        }

        public void DisplayErrorMessage(string errorMessage)
        {
            MessageBox.Show(errorMessage,
                Properties.Resources.MsgBoxCaption_Error,
                MessageBoxButton.OK, MessageBoxImage.Error
                );
        }

        public void DisplaySuccessMessage(string successMessage)
        {
            MessageBox.Show(successMessage,
                Properties.Resources.MsgBoxCaption_Success,
                MessageBoxButton.OK, MessageBoxImage.Information
                );
        }

        public void DisplayAboutBox()
        {
            _aboutWindow = new AboutWindow();
            _aboutWindow.Owner = Application.Current.MainWindow;
            _aboutWindow.ShowDialog();
        }
    }
}
