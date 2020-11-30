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
using System.ComponentModel;
using System.Windows.Input;
using System.Diagnostics;
using System.IO;
using System.Runtime.CompilerServices;
using System.Text;
using System.Windows.Media.Imaging;
using WpfAdBook.DAL;
using WpfAdBook.Services;

namespace WpfAdBook.ViewModels
{
    public class EditPersonWindowVM : INotifyPropertyChanged, IDisposable
    {
        private AdPersonVM _selectedPersonCopy, _selectedPerson;
        private IDialogService _dialogService;
        private IDataService _dataService;

        public ICommand OkCommand { get; }
        public ICommand CancelCommand { get; }
        public ICommand SelectPhotoCommand { get; }
        public ICommand ClearPhotoCommand { get; }
        public ICommand WindowClosedNotification { get; }
        public ICommand WindowLoadedNotification { get; }

        public EditPersonWindowVM(IDataService dataService, IDialogService dialogService)
        {
            _dialogService = dialogService;
            _dataService = dataService;
            ViewModelMediator.Default.Register<AdPersonVM>(this, OnPersonReceived);

            OkCommand = new CustomCommand(ExecuteOkCommand);
            CancelCommand = new CustomCommand(ExecuteCancelCommand);
            SelectPhotoCommand = new CustomCommand(ExecuteSelectPhotoCommand, CanExecuteSelectPhotoCommand);
            ClearPhotoCommand = new CustomCommand(ExecuteClearPhotoCommand, CanExecuteClearPhotoCommand);
            WindowClosedNotification = new CustomCommand(OnWindowClosed);
            WindowLoadedNotification = new CustomCommand(OnWindowLoaded);

            StringBuilder sb = new StringBuilder();
            foreach (var id in adbookcli.AdAttributes.AttrIds) {
                adbookcli.AdAttribute attr = adbookcli.AdAttributes.Get(id);
                sb.Append("DisplayName: ").Append(attr.DisplayName).Append("\n").Append("OID: ").Append(attr.Oid).Append("\n")
                    .Append("LDAP Name: ").Append(attr.LdapName).Append("\n\n");
            }
            AttributeDetails = sb.ToString();
        }

        private void OnWindowLoaded(object obj)
        {
            _dialogService.RestoreEditPersonWindowState();
        }

        private bool CanExecuteSelectPhotoCommand(object arg)
        {
            if (_selectedPersonCopy != null) {
                return _selectedPersonCopy.IsAttributeWrittable(adbookcli.AttrId.ThumbnailPhoto);
            }
            return false;
        }

        private bool CanExecuteClearPhotoCommand(object arg)
        {
            if (_selectedPersonCopy != null) {
                bool photoExists = (_selectedPersonCopy.BinaryAttrs[AdAttributesVM.ThumbnailPhoto.LdapName].Value != null) &&
                    (_selectedPersonCopy.BinaryAttrs[AdAttributesVM.ThumbnailPhoto.LdapName].Value.Length > 0);

                return photoExists && _selectedPersonCopy.IsAttributeWrittable(adbookcli.AttrId.ThumbnailPhoto);
            }
            return false;
        }

        private void OnWindowClosed(object obj)
        {
            _dialogService.SaveEditPersonWindowState();
            _selectedPersonCopy?.Dispose();
            _selectedPersonCopy = null;
        }

        public bool IsAttributeWrittable(adbookcli.AttrId attrId)
        {
            return _selectedPersonCopy.IsAttributeWrittable(attrId);
        }

        public bool IsAttributeReadOnly(adbookcli.AttrId attrId)
        {
            return !_selectedPersonCopy.IsAttributeWrittable(attrId);
        }

        public string AttributeDetails { get; set; }

        private void ExecuteSelectPhotoCommand(object obj)
        {
            string filePath = _dialogService.SelectPhoto();
            if (string.IsNullOrWhiteSpace(filePath)) {
                return;
            }

            // Open a stream and decode a JPEG image
            try {
                byte[] photoData = File.ReadAllBytes(filePath);
                if (photoData.Length > AdAttributesVM.ThumbnailPhoto.MaxLen) {
                    throw new ArgumentOutOfRangeException(filePath, "The file size exceeds the limit.");
                }
                Stream imageStreamSource = new FileStream(filePath, FileMode.Open, FileAccess.Read, FileShare.Read);
                var decoder = new JpegBitmapDecoder(imageStreamSource, BitmapCreateOptions.PreservePixelFormat, BitmapCacheOption.Default);

                _selectedPersonCopy.BinaryAttrs[AdAttributesVM.ThumbnailPhoto.LdapName].Value = photoData;
            }
            catch (FileFormatException formatError) {
                _dialogService.DisplayErrorMessage(formatError.Message);
            }
            catch (ArgumentOutOfRangeException error) {
                _dialogService.DisplayErrorMessage(error.Message);
            }
        }

        private void ExecuteClearPhotoCommand(object obj)
        {
            _selectedPersonCopy.BinaryAttrs[AdAttributesVM.ThumbnailPhoto.LdapName].Value = new byte[0];
        }

        private void ExecuteCancelCommand(object obj)
        {
            _dialogService.CloseEditPersonWindow();
        }

        private void UploadChangedData()
        {
            _dataService.WriteAdPerson(_selectedPersonCopy);
        }

        private void ExecuteOkCommand(object obj)
        {
            try {
                UploadChangedData();
            }
            catch (RepositoryInaccessibleError exc) {
                _dialogService.DisplayErrorMessage(exc.Message);
            }
            catch (DataModificationError exc) {
                _dialogService.DisplayErrorMessage(exc.Message);
            }
            catch (Exception exc) {
                _dialogService.DisplayErrorMessage(exc.Message);
                throw;
            }
            _selectedPerson.CopyFrom(_selectedPersonCopy);
            _dialogService.CloseEditPersonWindow();
        }

        public AdPersonVM SelectedPerson {
            get {
                return _selectedPersonCopy;
            }
            set {
                _selectedPersonCopy = value;
                RaisePropertyChanged(nameof(SelectedPerson));
            }
        }

        private void OnPersonReceived(AdPersonVM person)
        {
            _selectedPerson = person;
            SelectedPerson = (AdPersonVM)person.Clone();
        }

        #region INotifyPropertyChanged implementation

        public event PropertyChangedEventHandler PropertyChanged;

        private void RaisePropertyChanged([CallerMemberName]string propertyName = "")
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }

        #region IDisposable Support

        private bool disposedValue = false; // To detect redundant calls

        protected virtual void Dispose(bool disposing)
        {
            if (!disposedValue) {
                if (disposing) {
                    _selectedPersonCopy?.Dispose();
                    _selectedPersonCopy = null;
                }
                disposedValue = true;
            }
        }
        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        #endregion IDisposable Support

        #endregion INotifyPropertyChanged implementation
    }
}