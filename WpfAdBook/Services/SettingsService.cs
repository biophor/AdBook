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
using System.Collections.ObjectModel;
using System.Runtime.InteropServices;
using System.Security;
using System.Security.Cryptography;
using System.Text;
using WpfAdBook.ViewModels;

namespace WpfAdBook.Services
{
    public class SettingsService : ISettingsService
    {
        private AdAccessFactory _adAccessFactory = AdAccessFactory.CreateInstance();
        private ConnectionParams _connectionParams = new ConnectionParams();

        public ConnectionParams ConnectionParams {
            get {
                return _connectionParams;
            }
            set {
                if (_connectionParams != value) {
                    _connectionParams = value;
                }
            }
        }
        public FilteringParams FilteringParams { get; set; } = new FilteringParams();
        public ObservableCollection<AdPersonVM> SavedSearchResult { get; set; } = new ObservableCollection<AdPersonVM>();

        public System.Drawing.Size MainWndSize { get; set; } = new System.Drawing.Size(500, 700);
        public System.Drawing.Point MainWndPos { get; set; } = new System.Drawing.Point(100, 100);
        public bool MainWndMaximized { get; set; } = false;

        public SettingsService(bool autoLoadSettings)
        {
            if (autoLoadSettings) {
                Load();
            }
        }

        static private string SecureStringToProtectedString(SecureString secureString)
        {
            if (secureString.Length == 0) {
                throw new ArgumentException(nameof(secureString));
            }
            int sizeInBytes = secureString.Length * 2;
            IntPtr unmanagedBuffer = IntPtr.Zero;
            try {
                unmanagedBuffer = Marshal.SecureStringToGlobalAllocUnicode(secureString);
                byte[] arrToProtect = new byte[sizeInBytes];
                Marshal.Copy(unmanagedBuffer, arrToProtect, 0, sizeInBytes);
                byte[] arrProtected = ProtectedData.Protect(arrToProtect, null,
                    DataProtectionScope.CurrentUser);
                return Convert.ToBase64String(arrProtected);
            }
            finally {
                if (unmanagedBuffer != IntPtr.Zero) {
                    Marshal.ZeroFreeGlobalAllocUnicode(unmanagedBuffer);
                }
            }
        }

        static private SecureString ProtectedStringToSecureString(string protectedString)
        {
            if (String.IsNullOrWhiteSpace(protectedString)) {
                throw new ArgumentException();
            }
            SecureString secureString = new SecureString();
            byte[] arrProtected = null;
            try {
                arrProtected = Convert.FromBase64String(protectedString);
            }
            catch (System.FormatException) {
                return secureString;
            }
            try {
                byte[] arrUnprotected = ProtectedData.Unprotect(arrProtected, null,
                    DataProtectionScope.CurrentUser
                );
                foreach (char ch in Encoding.Unicode.GetChars(arrUnprotected)) {
                    secureString.AppendChar(ch);
                }
            }
            catch (CryptographicException) {
                return secureString;
            }
            return secureString;
        }

        public void Save()
        {
            Properties.Settings.Default.Login = ConnectionParams.Login;
            if (ConnectionParams.Password != null && ConnectionParams.Password.Length > 0) {
                Properties.Settings.Default.Password = SecureStringToProtectedString(ConnectionParams.Password);
            }
            else {
                Properties.Settings.Default.Password = "";
            }
            Properties.Settings.Default.DomainOrDc = ConnectionParams.Dc;
            Properties.Settings.Default.ConnectToDefaultDomain = ConnectionParams.UseDomainYouAreLoggedIn;
            Properties.Settings.Default.UseCurrentUserCredentials = ConnectionParams.UseCurrentUserCredentials;
            Properties.Settings.Default.FilterRules = FilteringParams.SerializedFilterRules;
            Properties.Settings.Default.StrictFiltering = FilteringParams.StrictFiltering;
            Properties.Settings.Default.MainWndSize = MainWndSize;
            Properties.Settings.Default.MainWndPos = MainWndPos;
            Properties.Settings.Default.MainWndMaximized = MainWndMaximized;
            Properties.Settings.Default.Save();

            using (var adplk = _adAccessFactory.GetPersonDescKeeper()) {
                foreach (AdPersonVM adp in SavedSearchResult) {
                    adplk.Add(adp.GetUnderlyingType());
                }
                adplk.SetNameByConnectionParams(_connectionParams);
                adplk.Save();
            }
        }

        public void Load()
        {
            ConnectionParams.Login = Properties.Settings.Default.Login;
            if (!string.IsNullOrWhiteSpace(Properties.Settings.Default.Password)) {
                ConnectionParams.Password = ProtectedStringToSecureString(Properties.Settings.Default.Password);
            }
            else {
                ConnectionParams.Password = new SecureString();
            }
            ConnectionParams.Dc = Properties.Settings.Default.DomainOrDc;
            ConnectionParams.UseDomainYouAreLoggedIn = Properties.Settings.Default.ConnectToDefaultDomain;
            ConnectionParams.UseCurrentUserCredentials = Properties.Settings.Default.UseCurrentUserCredentials;
            FilteringParams.SerializedFilterRules = Properties.Settings.Default.FilterRules;
            FilteringParams.StrictFiltering = Properties.Settings.Default.StrictFiltering;
            MainWndSize = Properties.Settings.Default.MainWndSize;
            MainWndPos = Properties.Settings.Default.MainWndPos;
            MainWndMaximized = Properties.Settings.Default.MainWndMaximized;

            using (var adplk = _adAccessFactory.GetPersonDescKeeper()) {
                adplk.SetNameByConnectionParams(_connectionParams);
                adplk.Load();
                foreach (AdPerson adp in adplk) {
                    SavedSearchResult.Add(new AdPersonVM(adp));
                }
            }
        }

        public static string SecureStringToPlainString(SecureString secureString)
        {
            if (secureString.Length == 0) {
                return string.Empty;
            }
            int sizeInBytes = secureString.Length * 2;
            IntPtr unmanagedBuffer = IntPtr.Zero;
            try {
                unmanagedBuffer = Marshal.SecureStringToGlobalAllocUnicode(secureString);
                byte[] decodedByteBuffer = new byte[sizeInBytes];
                Marshal.Copy(unmanagedBuffer, decodedByteBuffer, 0, sizeInBytes);
                char[] charBuffer = Encoding.Unicode.GetChars(decodedByteBuffer);
                return new string(charBuffer);
            }
            finally {
                if (unmanagedBuffer != IntPtr.Zero) {
                    Marshal.ZeroFreeGlobalAllocUnicode(unmanagedBuffer);
                }
            }
        }

        #region IDisposable Support

        private bool disposedValue = false;

        protected virtual void Dispose(bool disposing)
        {
            if (!disposedValue) {
                if (disposing) {
                    _adAccessFactory.Dispose();
                    _adAccessFactory = null;
                    _connectionParams.Dispose();
                    _connectionParams = null;
                    adbookcli.AdAccessFactory.EnsureDisposed();
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
    }
}