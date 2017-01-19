// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using adbookcli;

namespace WfAdBook
{
    public partial class ModifyAttrForm : Form
    {
        private AdPerson _adp;
        private AdAttribute _attr;
        
        public ModifyAttrForm(AdPerson adp, AdAttribute attr, string currentValue)
        {
            InitializeComponent();
            _adp = adp;
            _attr = attr;
            tboxCurrentValue.Text = currentValue;
            tboxNewValue.Text = currentValue;
            tboxObjectPath.Text = adp.Dn;
            var sb = new StringBuilder();
            sb.Append(attr.DisplayName).Append(" / ").Append(attr.LdapName).Append(" / ").Append(attr.Oid);
            tboxAttrName.Text = sb.ToString();
            Location = Properties.Settings.Default.ModifyAttrFormLocation;
        }
        private void ApplyNewValue()
        {
            using (var cs = new ConnectionSettings()) {
                using (var adc = new AdConnector(cs, _adp.Dn)) {
                    try {
                        adc.Connect();
                        if (_attr.Id == AttrId.CommonName) {
                            if (string.IsNullOrWhiteSpace(tboxNewValue.Text)) {
                                throw new Exception(Properties.Resources.ErrorInvalidValueMsgBoxText);
                            }
                            string newValue = tboxNewValue.Text.Trim();
                            adc.Rename(newValue);
                            _adp.set_StringAttr(_attr.LdapName, newValue);
                            var dnAttr = AdAttributes.Get(AttrId.Dn);
                            _adp.set_StringAttr(dnAttr.LdapName, adc.DownloadStringAttr(dnAttr.LdapName));
                        }
                        else {
                            adc.UploadStringAttr(_attr.LdapName, tboxNewValue.Text);
                            _adp.set_StringAttr(_attr.LdapName, tboxNewValue.Text);
                        }
                    }
                    catch (Exception exc) {
                        MessageBox.Show(this, exc.Message, Properties.Resources.ErrorMsgBoxTitle,
                            MessageBoxButtons.OK, MessageBoxIcon.Error
                            );
                    }
                }
            }
        }
        private void buttonOk_Click(object sender, EventArgs e)
        {
            ApplyNewValue();
        }
        private void ModifyAttrForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            Properties.Settings.Default.ModifyAttrFormLocation = Location;
        }
    }
}
