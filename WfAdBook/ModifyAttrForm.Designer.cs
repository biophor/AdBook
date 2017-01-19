namespace WfAdBook
{
    partial class ModifyAttrForm
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
            this.buttonOk = new System.Windows.Forms.Button();
            this.buttonCancel = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.tboxAttrName = new System.Windows.Forms.TextBox();
            this.tboxCurrentValue = new System.Windows.Forms.TextBox();
            this.tboxNewValue = new System.Windows.Forms.TextBox();
            this.tboxObjectPath = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // buttonOk
            // 
            this.buttonOk.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonOk.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.buttonOk.Location = new System.Drawing.Point(109, 197);
            this.buttonOk.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.buttonOk.Name = "buttonOk";
            this.buttonOk.Size = new System.Drawing.Size(75, 23);
            this.buttonOk.TabIndex = 8;
            this.buttonOk.Text = "OK";
            this.buttonOk.UseVisualStyleBackColor = true;
            this.buttonOk.Click += new System.EventHandler(this.buttonOk_Click);
            // 
            // buttonCancel
            // 
            this.buttonCancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.buttonCancel.Location = new System.Drawing.Point(188, 197);
            this.buttonCancel.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.buttonCancel.Name = "buttonCancel";
            this.buttonCancel.Size = new System.Drawing.Size(75, 23);
            this.buttonCancel.TabIndex = 9;
            this.buttonCancel.Text = "Cancel";
            this.buttonCancel.UseVisualStyleBackColor = true;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(10, 11);
            this.label1.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(207, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "Attribute display name / Ldap name / OID:";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(10, 54);
            this.label2.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(73, 13);
            this.label2.TabIndex = 2;
            this.label2.Text = "Current value:";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(10, 100);
            this.label3.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(61, 13);
            this.label3.TabIndex = 4;
            this.label3.Text = "New value:";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(10, 147);
            this.label4.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(41, 13);
            this.label4.TabIndex = 6;
            this.label4.Text = "Object:";
            // 
            // tboxAttrName
            // 
            this.tboxAttrName.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.tboxAttrName.Location = new System.Drawing.Point(12, 27);
            this.tboxAttrName.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.tboxAttrName.MinimumSize = new System.Drawing.Size(226, 4);
            this.tboxAttrName.Name = "tboxAttrName";
            this.tboxAttrName.ReadOnly = true;
            this.tboxAttrName.Size = new System.Drawing.Size(254, 20);
            this.tboxAttrName.TabIndex = 1;
            // 
            // tboxCurrentValue
            // 
            this.tboxCurrentValue.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.tboxCurrentValue.Location = new System.Drawing.Point(12, 70);
            this.tboxCurrentValue.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.tboxCurrentValue.Name = "tboxCurrentValue";
            this.tboxCurrentValue.ReadOnly = true;
            this.tboxCurrentValue.Size = new System.Drawing.Size(254, 20);
            this.tboxCurrentValue.TabIndex = 3;
            // 
            // tboxNewValue
            // 
            this.tboxNewValue.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.tboxNewValue.Location = new System.Drawing.Point(12, 117);
            this.tboxNewValue.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.tboxNewValue.Name = "tboxNewValue";
            this.tboxNewValue.Size = new System.Drawing.Size(254, 20);
            this.tboxNewValue.TabIndex = 5;
            // 
            // tboxObjectPath
            // 
            this.tboxObjectPath.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.tboxObjectPath.Location = new System.Drawing.Point(12, 163);
            this.tboxObjectPath.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.tboxObjectPath.Name = "tboxObjectPath";
            this.tboxObjectPath.ReadOnly = true;
            this.tboxObjectPath.Size = new System.Drawing.Size(254, 20);
            this.tboxObjectPath.TabIndex = 7;
            // 
            // ModifyAttrForm
            // 
            this.AcceptButton = this.buttonOk;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.buttonCancel;
            this.ClientSize = new System.Drawing.Size(274, 231);
            this.Controls.Add(this.tboxObjectPath);
            this.Controls.Add(this.tboxNewValue);
            this.Controls.Add(this.tboxCurrentValue);
            this.Controls.Add(this.tboxAttrName);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.buttonCancel);
            this.Controls.Add(this.buttonOk);
            this.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.MaximizeBox = false;
            this.Name = "ModifyAttrForm";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.SizeGripStyle = System.Windows.Forms.SizeGripStyle.Show;
            this.StartPosition = System.Windows.Forms.FormStartPosition.Manual;
            this.Text = "Attribute Editor";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.ModifyAttrForm_FormClosed);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button buttonOk;
        private System.Windows.Forms.Button buttonCancel;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox tboxAttrName;
        private System.Windows.Forms.TextBox tboxCurrentValue;
        private System.Windows.Forms.TextBox tboxNewValue;
        private System.Windows.Forms.TextBox tboxObjectPath;
    }
}