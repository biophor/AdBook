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
using System.Reflection;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Configuration;
using System.Windows.Forms;
using System.Security;
using System.Runtime.InteropServices;
using adbookcli;

namespace WfAdBook
{
    public partial class SettingsForm : Form //-V3073
    {        
        private FontEntryItem ContactDetailsEntryItem { get; set; }
        private FontEntryItem ContactListEntryItem { get; set; }
        private FontEntryItem FilterListEntryItem { get; set; }
        private class FontEntryItem: IDisposable
        {
            public FontEntryItem(string entryName) // default font
            {
                EntryFont = null;
                EntryName = entryName;
            }
            public FontEntryItem(Font font, string entryName)
            {
                EntryFont = font;
                EntryName = entryName;
            }
            public override string ToString()
            {
                return EntryName;
            }
            public void Dispose()
            {
                EntryFont?.Dispose();
                EntryFont = null;
            }
            public Font EntryFont { get; set; }
            public string EntryName { get; set; }
        }
        public SettingsForm()
        {
            InitializeComponent();
            Location = Properties.Settings.Default.SettingsFormLocation;            
            LoadSettings();
            Disposed += SettingsForm_Disposed;
        }

        private void SettingsForm_Disposed(object sender, EventArgs e)
        {
            ContactDetailsEntryItem.Dispose();
            ContactListEntryItem.Dispose();
            FilterListEntryItem.Dispose();
        }

        private bool VerifyConnectionSettings()
        {
            if (rbuttonSpecifyDomain.Checked == true) {
                if (string.IsNullOrWhiteSpace(tboxDc.Text)) {
                    errorProvider.SetIconAlignment(tboxDc, ErrorIconAlignment.MiddleLeft);
                    errorProvider.SetError(tboxDc, Properties.Resources.ErrorDcIsEmpty);
                    return false;
                }
            }
            if (rbuttonSpecifyCred.Checked == true) {
                if (string.IsNullOrWhiteSpace(tboxUserName.Text.Trim())) {
                    errorProvider.SetIconAlignment(labelUserName, ErrorIconAlignment.MiddleLeft);
                    errorProvider.SetError(tboxUserName, Properties.Resources.ErrorLoginIsEmpty);
                    return false;
                }
                if (string.IsNullOrEmpty(tboxPassword.Text)) {
                    errorProvider.SetIconAlignment(labelPassword, ErrorIconAlignment.MiddleLeft);
                    errorProvider.SetError(labelPassword, Properties.Resources.ErrorPasswordIsEmpty);
                    return false;
                }
            }
            return true;
        }
        private void buttonOK_Click(object sender, EventArgs e)
        {
            if (!VerifyConnectionSettings()) {
                return;
            }
            SaveSettings();            
            Close();
        }
        SecureString StringToSecureString(string s)
        {
            SecureString ret = new SecureString();
            foreach (var ch in s) {
                ret.AppendChar(ch);
            }
            return ret;
        }
        string SecureStringToString(SecureString secureString)
        {
            IntPtr unmanagedBuffer = IntPtr.Zero;
            try {
                unmanagedBuffer = Marshal.SecureStringToGlobalAllocUnicode(secureString);
                return Marshal.PtrToStringUni(unmanagedBuffer);
            }
            finally {
                Marshal.ZeroFreeGlobalAllocUnicode(unmanagedBuffer);
            }
        }
        private void SaveConnectionSettings()
        {
            using (ConnectionSettings cs = new ConnectionSettings()) {
                cs.Dc = tboxDc.Text;
                cs.Login = tboxUserName.Text;
                cs.Password = StringToSecureString(tboxPassword.Text);
                cs.UseCurrentUserCredentials = rbuttonCurUserCred.Checked;
                cs.UseDomainYouAreLoggedIn = rbuttonDefaultDomain.Checked;
                cs.Save();
            }
        }
        private void SaveFontSettings()
        {
            FontSettings fontSettings = new FontSettings();
            fontSettings.ContactDetailsFont = ContactDetailsEntryItem.EntryFont;
            fontSettings.ContactListFont = ContactListEntryItem.EntryFont;
            fontSettings.FilterListFont = FilterListEntryItem.EntryFont;
            fontSettings.Save();
        }
        private void SaveSettings()
        {
            SaveConnectionSettings();
            SaveFontSettings();
        }
        private void LoadConnectionSettings()
        {
            using (ConnectionSettings cs = new ConnectionSettings()) {                
                tboxDc.Text = cs.Dc;
                tboxUserName.Text = cs.Login;
                if (cs.Password.Length > 0) {
                    tboxPassword.Text = SecureStringToString(cs.Password);
                }
                else {
                    tboxPassword.Text = "";
                }
                rbuttonCurUserCred.Checked = cs.UseCurrentUserCredentials;
                rbuttonSpecifyCred.Checked = !cs.UseCurrentUserCredentials;
                rbuttonDefaultDomain.Checked = cs.UseDomainYouAreLoggedIn;
                rbuttonSpecifyDomain.Checked = !cs.UseDomainYouAreLoggedIn;
            }
        }
        private void LoadFontSettings()
        {
            FontSettings fontSettings = new FontSettings();

            FilterListEntryItem = new FontEntryItem(
                fontSettings.FilterListFont,
                Properties.Resources.FilterListFontEntry
            );
            ContactListEntryItem = new FontEntryItem(
                fontSettings.ContactListFont,
                Properties.Resources.ContactListFontEntry
            );            
            ContactDetailsEntryItem = new FontEntryItem(
                fontSettings.ContactDetailsFont,
                Properties.Resources.ContactDetailsFontEntry                
            );
            lbox.Items.Add(FilterListEntryItem);
            lbox.Items.Add(ContactListEntryItem);
            lbox.Items.Add(ContactDetailsEntryItem);
            lbox.SelectedIndex = 0;
        }
        void LoadSettings()
        {
            LoadConnectionSettings();
            LoadFontSettings();
        }
        private void buttonCancel_Click(object sender, EventArgs e)
        {
            Close();
        }
        private void SettingsForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            Properties.Settings.Default.SettingsFormLocation = Location;
        }
        private void radioButtonSpecifyDomain_CheckedChanged(object sender, EventArgs e)
        {
            tboxDc.Enabled = rbuttonSpecifyDomain.Checked;
        }
        private void rbuttonSpecifyCred_CheckedChanged(object sender, EventArgs e)
        {            
            labelPassword.Enabled = rbuttonSpecifyCred.Checked;
            tboxPassword.Enabled = rbuttonSpecifyCred.Checked;
            labelUserName.Enabled = rbuttonSpecifyCred.Checked;
            tboxUserName.Enabled = rbuttonSpecifyCred.Checked;
        }
        private void buttonTestConnection_Click(object sender, EventArgs e)
        {
            if (!VerifyConnectionSettings()) {
                return;
            }
            using (ConnectionSettings cs = new ConnectionSettings()) {
                cs.Dc = tboxDc.Text;
                cs.Login = tboxUserName.Text;
                cs.Password = StringToSecureString(tboxPassword.Text);
                cs.UseCurrentUserCredentials = rbuttonCurUserCred.Checked;
                cs.UseDomainYouAreLoggedIn = rbuttonDefaultDomain.Checked;

                using (var adc = adbookcli.AdAccessFactory.GetInstance().CreateConnector()) {
                    try {
                        adc.Connect(cs);
                    }
                    catch (Exception exc) {
                        MessageBox.Show(exc.Message, Properties.Resources.ErrorMsgBoxTitle,
                            MessageBoxButtons.OK, MessageBoxIcon.Error
                            );
                        return;
                    }
                    MessageBox.Show(Properties.Resources.ConnectionSucceededMsgBoxText,
                        Properties.Resources.ConnectionSucceededMsgBoxTitle,
                        MessageBoxButtons.OK, MessageBoxIcon.Information
                        );
                }
            }
        }
        private void lbox_SelectedIndexChanged(object sender, EventArgs e)
        {
            DisplaySelectedFont();
        }
        private void DisplaySelectedFont()
        {
            if (0 == lbox.SelectedItems.Count) {
                return;
            }
            var selItem = (FontEntryItem)lbox.SelectedItems[0];
            if (selItem.EntryFont == null) {
                labelFontPreview.Text = Properties.Resources.FontIsNotSetMsg;
                return;
            }
            labelFontPreview.Font = selItem.EntryFont;
            labelFontPreview.Text = selItem.EntryFont.FontFamily.Name + " " + 
                selItem.EntryFont.SizeInPoints.ToString();
        }
        private void buttonSelectFont_Click(object sender, EventArgs e)
        {
            if (0 == lbox.SelectedItems.Count) {
                return;
            }
            if (fontDialog.ShowDialog() != DialogResult.OK) {
                return;
            }
            var selItem = (FontEntryItem)lbox.SelectedItems[0];
            selItem.EntryFont = fontDialog.Font;
            DisplaySelectedFont();
        }
    }
}
