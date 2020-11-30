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
using System.Configuration;
using System.Security.Cryptography;
using System.Runtime.InteropServices;
using System.Security;

namespace WfAdBook
{
    sealed class ConnectionSettings: adbookcli.ConnectionParams
    {
        public ConnectionSettings()
        {
            Load();
        }
        public bool HasPassword{
            get {
                return Password != null && Password.Length > 0;
            }
        }
        public bool HasLogin {
            get {
                return Login != null && Login.Length > 0;
            }
        }
        static private string SecureStringToProtectedString(SecureString secureString)
        {
            if (secureString.Length == 0) {
                throw new ArgumentException();
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
                Marshal.ZeroFreeGlobalAllocUnicode(unmanagedBuffer);
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
            catch (System.FormatException) {    // garbage
                return secureString;
            }
            byte[] arrUnprotected = ProtectedData.Unprotect(arrProtected, null,
                DataProtectionScope.CurrentUser
                );
            foreach (char ch in Encoding.Unicode.GetChars(arrUnprotected)) {
                secureString.AppendChar(ch);
            }
            return secureString;
        }
        public void Save()
        {
            Properties.Settings.Default.Login = Login;
            if (HasPassword) {
                Properties.Settings.Default.Password = SecureStringToProtectedString(Password);
            }
            else {
                Properties.Settings.Default.Password = "";
            }
            Properties.Settings.Default.DomainOrDc = Address;
            Properties.Settings.Default.ConnectDefaultDomain = ConnectDomainYouLoggedIn;
            Properties.Settings.Default.UseCurrentUserCredentials = UseCurrentUserCredentials;
            Properties.Settings.Default.Save();
        }
        public void Load()
        {
            Login = Properties.Settings.Default.Login;
            if (!string.IsNullOrWhiteSpace(Properties.Settings.Default.Password)) {
                Password = ProtectedStringToSecureString(Properties.Settings.Default.Password);
            }
            else {
                Password = new SecureString();
            }
            Address = Properties.Settings.Default.DomainOrDc;
            ConnectDomainYouLoggedIn = Properties.Settings.Default.ConnectDefaultDomain;
            UseCurrentUserCredentials = Properties.Settings.Default.UseCurrentUserCredentials;
        }
    }
}
