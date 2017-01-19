namespace WfAdBook
{
    partial class SettingsForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null)) {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.buttonOK = new System.Windows.Forms.Button();
            this.buttonCancel = new System.Windows.Forms.Button();
            this.tabControl = new System.Windows.Forms.TabControl();
            this.tabPageConnection = new System.Windows.Forms.TabPage();
            this.panel1 = new System.Windows.Forms.Panel();
            this.tboxPassword = new System.Windows.Forms.TextBox();
            this.labelPassword = new System.Windows.Forms.Label();
            this.labelUserName = new System.Windows.Forms.Label();
            this.tboxUserName = new System.Windows.Forms.TextBox();
            this.rbuttonSpecifyCred = new System.Windows.Forms.RadioButton();
            this.rbuttonCurUserCred = new System.Windows.Forms.RadioButton();
            this.tboxDc = new System.Windows.Forms.TextBox();
            this.rbuttonSpecifyDomain = new System.Windows.Forms.RadioButton();
            this.rbuttonDefaultDomain = new System.Windows.Forms.RadioButton();
            this.buttonTestConnection = new System.Windows.Forms.Button();
            this.tabPageFonts = new System.Windows.Forms.TabPage();
            this.lbox = new System.Windows.Forms.ListBox();
            this.labelFontPreview = new System.Windows.Forms.Label();
            this.buttonSelectFont = new System.Windows.Forms.Button();
            this.errorProvider = new System.Windows.Forms.ErrorProvider(this.components);
            this.fontDialog = new System.Windows.Forms.FontDialog();
            this.tabControl.SuspendLayout();
            this.tabPageConnection.SuspendLayout();
            this.panel1.SuspendLayout();
            this.tabPageFonts.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.errorProvider)).BeginInit();
            this.SuspendLayout();
            // 
            // buttonOK
            // 
            this.buttonOK.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonOK.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.buttonOK.Location = new System.Drawing.Point(124, 274);
            this.buttonOK.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.buttonOK.Name = "buttonOK";
            this.buttonOK.Size = new System.Drawing.Size(75, 23);
            this.buttonOK.TabIndex = 1;
            this.buttonOK.Text = "OK";
            this.buttonOK.UseVisualStyleBackColor = true;
            this.buttonOK.Click += new System.EventHandler(this.buttonOK_Click);
            // 
            // buttonCancel
            // 
            this.buttonCancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.buttonCancel.Location = new System.Drawing.Point(203, 274);
            this.buttonCancel.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.buttonCancel.Name = "buttonCancel";
            this.buttonCancel.Size = new System.Drawing.Size(75, 23);
            this.buttonCancel.TabIndex = 2;
            this.buttonCancel.Text = "Cancel";
            this.buttonCancel.UseVisualStyleBackColor = true;
            this.buttonCancel.Click += new System.EventHandler(this.buttonCancel_Click);
            // 
            // tabControl
            // 
            this.tabControl.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.tabControl.Controls.Add(this.tabPageConnection);
            this.tabControl.Controls.Add(this.tabPageFonts);
            this.tabControl.Location = new System.Drawing.Point(9, 10);
            this.tabControl.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.tabControl.Name = "tabControl";
            this.tabControl.SelectedIndex = 0;
            this.tabControl.Size = new System.Drawing.Size(269, 260);
            this.tabControl.TabIndex = 0;
            // 
            // tabPageConnection
            // 
            this.tabPageConnection.Controls.Add(this.panel1);
            this.tabPageConnection.Controls.Add(this.tboxDc);
            this.tabPageConnection.Controls.Add(this.rbuttonSpecifyDomain);
            this.tabPageConnection.Controls.Add(this.rbuttonDefaultDomain);
            this.tabPageConnection.Controls.Add(this.buttonTestConnection);
            this.tabPageConnection.Location = new System.Drawing.Point(4, 22);
            this.tabPageConnection.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.tabPageConnection.Name = "tabPageConnection";
            this.tabPageConnection.Padding = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.tabPageConnection.Size = new System.Drawing.Size(261, 234);
            this.tabPageConnection.TabIndex = 0;
            this.tabPageConnection.Text = "Connection";
            this.tabPageConnection.UseVisualStyleBackColor = true;
            // 
            // panel1
            // 
            this.panel1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.panel1.Controls.Add(this.tboxPassword);
            this.panel1.Controls.Add(this.labelPassword);
            this.panel1.Controls.Add(this.labelUserName);
            this.panel1.Controls.Add(this.tboxUserName);
            this.panel1.Controls.Add(this.rbuttonSpecifyCred);
            this.panel1.Controls.Add(this.rbuttonCurUserCred);
            this.panel1.Location = new System.Drawing.Point(5, 82);
            this.panel1.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(254, 94);
            this.panel1.TabIndex = 5;
            // 
            // tboxPassword
            // 
            this.tboxPassword.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.tboxPassword.Enabled = false;
            this.tboxPassword.Location = new System.Drawing.Point(80, 67);
            this.tboxPassword.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.tboxPassword.Name = "tboxPassword";
            this.tboxPassword.Size = new System.Drawing.Size(173, 20);
            this.tboxPassword.TabIndex = 5;
            this.tboxPassword.UseSystemPasswordChar = true;
            // 
            // labelPassword
            // 
            this.labelPassword.AutoSize = true;
            this.labelPassword.Enabled = false;
            this.labelPassword.Location = new System.Drawing.Point(18, 69);
            this.labelPassword.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.labelPassword.Name = "labelPassword";
            this.labelPassword.Size = new System.Drawing.Size(56, 13);
            this.labelPassword.TabIndex = 4;
            this.labelPassword.Text = "&Password:";
            // 
            // labelUserName
            // 
            this.labelUserName.AutoSize = true;
            this.labelUserName.Enabled = false;
            this.labelUserName.Location = new System.Drawing.Point(18, 46);
            this.labelUserName.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.labelUserName.Name = "labelUserName";
            this.labelUserName.Size = new System.Drawing.Size(58, 13);
            this.labelUserName.TabIndex = 2;
            this.labelUserName.Text = "&Username:";
            // 
            // tboxUserName
            // 
            this.tboxUserName.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.tboxUserName.Enabled = false;
            this.tboxUserName.Location = new System.Drawing.Point(80, 44);
            this.tboxUserName.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.tboxUserName.Name = "tboxUserName";
            this.tboxUserName.Size = new System.Drawing.Size(173, 20);
            this.tboxUserName.TabIndex = 3;
            // 
            // rbuttonSpecifyCred
            // 
            this.rbuttonSpecifyCred.AutoSize = true;
            this.rbuttonSpecifyCred.Location = new System.Drawing.Point(2, 22);
            this.rbuttonSpecifyCred.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.rbuttonSpecifyCred.Name = "rbuttonSpecifyCred";
            this.rbuttonSpecifyCred.Size = new System.Drawing.Size(117, 17);
            this.rbuttonSpecifyCred.TabIndex = 1;
            this.rbuttonSpecifyCred.TabStop = true;
            this.rbuttonSpecifyCred.Text = "Specify credentials:";
            this.rbuttonSpecifyCred.UseVisualStyleBackColor = true;
            this.rbuttonSpecifyCred.CheckedChanged += new System.EventHandler(this.rbuttonSpecifyCred_CheckedChanged);
            // 
            // rbuttonCurUserCred
            // 
            this.rbuttonCurUserCred.AutoSize = true;
            this.rbuttonCurUserCred.Checked = true;
            this.rbuttonCurUserCred.Location = new System.Drawing.Point(2, 2);
            this.rbuttonCurUserCred.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.rbuttonCurUserCred.Name = "rbuttonCurUserCred";
            this.rbuttonCurUserCred.Size = new System.Drawing.Size(176, 17);
            this.rbuttonCurUserCred.TabIndex = 0;
            this.rbuttonCurUserCred.TabStop = true;
            this.rbuttonCurUserCred.Text = "Authenticate as the current user";
            this.rbuttonCurUserCred.UseVisualStyleBackColor = true;
            // 
            // tboxDc
            // 
            this.tboxDc.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.tboxDc.Enabled = false;
            this.tboxDc.Location = new System.Drawing.Point(26, 50);
            this.tboxDc.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.tboxDc.MaxLength = 200;
            this.tboxDc.Name = "tboxDc";
            this.tboxDc.Size = new System.Drawing.Size(234, 20);
            this.tboxDc.TabIndex = 2;
            // 
            // rbuttonSpecifyDomain
            // 
            this.rbuttonSpecifyDomain.AutoSize = true;
            this.rbuttonSpecifyDomain.Location = new System.Drawing.Point(8, 28);
            this.rbuttonSpecifyDomain.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.rbuttonSpecifyDomain.Name = "rbuttonSpecifyDomain";
            this.rbuttonSpecifyDomain.Size = new System.Drawing.Size(213, 17);
            this.rbuttonSpecifyDomain.TabIndex = 1;
            this.rbuttonSpecifyDomain.Text = "Specify the domain or domain controller:";
            this.rbuttonSpecifyDomain.UseVisualStyleBackColor = true;
            this.rbuttonSpecifyDomain.CheckedChanged += new System.EventHandler(this.radioButtonSpecifyDomain_CheckedChanged);
            // 
            // rbuttonDefaultDomain
            // 
            this.rbuttonDefaultDomain.AutoSize = true;
            this.rbuttonDefaultDomain.Checked = true;
            this.rbuttonDefaultDomain.Location = new System.Drawing.Point(8, 6);
            this.rbuttonDefaultDomain.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.rbuttonDefaultDomain.Name = "rbuttonDefaultDomain";
            this.rbuttonDefaultDomain.Size = new System.Drawing.Size(198, 17);
            this.rbuttonDefaultDomain.TabIndex = 0;
            this.rbuttonDefaultDomain.TabStop = true;
            this.rbuttonDefaultDomain.Text = "Connect to the domain you logged in";
            this.rbuttonDefaultDomain.UseVisualStyleBackColor = true;
            // 
            // buttonTestConnection
            // 
            this.buttonTestConnection.Location = new System.Drawing.Point(0, 189);
            this.buttonTestConnection.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.buttonTestConnection.Name = "buttonTestConnection";
            this.buttonTestConnection.Size = new System.Drawing.Size(63, 20);
            this.buttonTestConnection.TabIndex = 3;
            this.buttonTestConnection.Text = "Test";
            this.buttonTestConnection.UseVisualStyleBackColor = true;
            this.buttonTestConnection.Click += new System.EventHandler(this.buttonTestConnection_Click);
            // 
            // tabPageFonts
            // 
            this.tabPageFonts.Controls.Add(this.lbox);
            this.tabPageFonts.Controls.Add(this.labelFontPreview);
            this.tabPageFonts.Controls.Add(this.buttonSelectFont);
            this.tabPageFonts.Location = new System.Drawing.Point(4, 22);
            this.tabPageFonts.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.tabPageFonts.Name = "tabPageFonts";
            this.tabPageFonts.Padding = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.tabPageFonts.Size = new System.Drawing.Size(261, 213);
            this.tabPageFonts.TabIndex = 2;
            this.tabPageFonts.Text = "Fonts";
            this.tabPageFonts.UseVisualStyleBackColor = true;
            // 
            // lbox
            // 
            this.lbox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.lbox.FormattingEnabled = true;
            this.lbox.Location = new System.Drawing.Point(7, 12);
            this.lbox.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.lbox.Name = "lbox";
            this.lbox.Size = new System.Drawing.Size(253, 56);
            this.lbox.TabIndex = 5;
            this.lbox.SelectedIndexChanged += new System.EventHandler(this.lbox_SelectedIndexChanged);
            // 
            // labelFontPreview
            // 
            this.labelFontPreview.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.labelFontPreview.Location = new System.Drawing.Point(7, 79);
            this.labelFontPreview.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.labelFontPreview.Name = "labelFontPreview";
            this.labelFontPreview.Size = new System.Drawing.Size(191, 124);
            this.labelFontPreview.TabIndex = 4;
            this.labelFontPreview.Text = "Font Preview";
            // 
            // buttonSelectFont
            // 
            this.buttonSelectFont.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonSelectFont.Location = new System.Drawing.Point(205, 79);
            this.buttonSelectFont.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.buttonSelectFont.Name = "buttonSelectFont";
            this.buttonSelectFont.Size = new System.Drawing.Size(56, 22);
            this.buttonSelectFont.TabIndex = 3;
            this.buttonSelectFont.Text = "Select...";
            this.buttonSelectFont.UseVisualStyleBackColor = true;
            this.buttonSelectFont.Click += new System.EventHandler(this.buttonSelectFont_Click);
            // 
            // errorProvider
            // 
            this.errorProvider.ContainerControl = this;
            // 
            // fontDialog
            // 
            this.fontDialog.AllowVerticalFonts = false;
            this.fontDialog.FontMustExist = true;
            this.fontDialog.ShowEffects = false;
            // 
            // SettingsForm
            // 
            this.AcceptButton = this.buttonOK;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.buttonCancel;
            this.ClientSize = new System.Drawing.Size(287, 303);
            this.Controls.Add(this.tabControl);
            this.Controls.Add(this.buttonCancel);
            this.Controls.Add(this.buttonOK);
            this.Location = new System.Drawing.Point(100, 100);
            this.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "SettingsForm";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.Manual;
            this.Text = "Settings";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.SettingsForm_FormClosing);
            this.tabControl.ResumeLayout(false);
            this.tabPageConnection.ResumeLayout(false);
            this.tabPageConnection.PerformLayout();
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.tabPageFonts.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.errorProvider)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button buttonOK;
        private System.Windows.Forms.Button buttonCancel;
        private System.Windows.Forms.TabControl tabControl;
        private System.Windows.Forms.TabPage tabPageConnection;
        private System.Windows.Forms.Button buttonTestConnection;
        private System.Windows.Forms.TabPage tabPageFonts;
        private System.Windows.Forms.TextBox tboxDc;
        private System.Windows.Forms.RadioButton rbuttonSpecifyDomain;
        private System.Windows.Forms.RadioButton rbuttonDefaultDomain;
        private System.Windows.Forms.RadioButton rbuttonCurUserCred;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.RadioButton rbuttonSpecifyCred;
        private System.Windows.Forms.TextBox tboxPassword;
        private System.Windows.Forms.Label labelPassword;
        private System.Windows.Forms.Label labelUserName;
        private System.Windows.Forms.TextBox tboxUserName;
        private System.Windows.Forms.ErrorProvider errorProvider;
        private System.Windows.Forms.FontDialog fontDialog;
        private System.Windows.Forms.Label labelFontPreview;
        private System.Windows.Forms.Button buttonSelectFont;
        private System.Windows.Forms.ListBox lbox;
    }
}