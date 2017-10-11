namespace WfAdBook
{
    partial class MainForm
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
            System.Windows.Forms.Label label2;
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
            this.buttonExit = new System.Windows.Forms.Button();
            this.splitContBottom = new System.Windows.Forms.SplitContainer();
            this.labelNumContacts = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.lviewResult = new System.Windows.Forms.ListView();
            this.splitContBottomRight = new System.Windows.Forms.SplitContainer();
            this.buttonChangeAttr = new System.Windows.Forms.Button();
            this.buttonCopyAttr = new System.Windows.Forms.Button();
            this.lviewDetails = new System.Windows.Forms.ListView();
            this.propertyName = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.propertyValue = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.buttonClearPhoto = new System.Windows.Forms.Button();
            this.buttonSelectPhoto = new System.Windows.Forms.Button();
            this.personPhoto = new System.Windows.Forms.PictureBox();
            this.label3 = new System.Windows.Forms.Label();
            this.buttonSettings = new System.Windows.Forms.Button();
            this.buttonAbout = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.cboxFilterNames = new System.Windows.Forms.ComboBox();
            this.cboxConditions = new System.Windows.Forms.ComboBox();
            this.cboxFilterValues = new System.Windows.Forms.ComboBox();
            this.buttonAddFilter = new System.Windows.Forms.Button();
            this.lviewFilters = new System.Windows.Forms.ListView();
            this.colFilterName = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.colCondition = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.colFilterValue = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.buttonRemoveFilter = new System.Windows.Forms.Button();
            this.buttonFind = new System.Windows.Forms.Button();
            this.splitCont = new System.Windows.Forms.SplitContainer();
            this.rbuttonAnyFilter = new System.Windows.Forms.RadioButton();
            this.rbuttonAllFilters = new System.Windows.Forms.RadioButton();
            this.selectPhotoDlg = new System.Windows.Forms.OpenFileDialog();
            label2 = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.splitContBottom)).BeginInit();
            this.splitContBottom.Panel1.SuspendLayout();
            this.splitContBottom.Panel2.SuspendLayout();
            this.splitContBottom.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContBottomRight)).BeginInit();
            this.splitContBottomRight.Panel1.SuspendLayout();
            this.splitContBottomRight.Panel2.SuspendLayout();
            this.splitContBottomRight.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.personPhoto)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.splitCont)).BeginInit();
            this.splitCont.Panel1.SuspendLayout();
            this.splitCont.Panel2.SuspendLayout();
            this.splitCont.SuspendLayout();
            this.SuspendLayout();
            // 
            // label2
            // 
            label2.AutoSize = true;
            label2.Dock = System.Windows.Forms.DockStyle.Top;
            label2.Location = new System.Drawing.Point(0, 0);
            label2.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            label2.Name = "label2";
            label2.Size = new System.Drawing.Size(42, 13);
            label2.TabIndex = 0;
            label2.Text = "Details:";
            // 
            // buttonExit
            // 
            this.buttonExit.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonExit.Location = new System.Drawing.Point(700, 527);
            this.buttonExit.Margin = new System.Windows.Forms.Padding(2);
            this.buttonExit.Name = "buttonExit";
            this.buttonExit.Size = new System.Drawing.Size(75, 23);
            this.buttonExit.TabIndex = 0;
            this.buttonExit.Text = "Exit";
            this.buttonExit.UseVisualStyleBackColor = true;
            this.buttonExit.Click += new System.EventHandler(this.buttonExit_Click);
            // 
            // splitContBottom
            // 
            this.splitContBottom.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContBottom.Location = new System.Drawing.Point(0, 0);
            this.splitContBottom.Margin = new System.Windows.Forms.Padding(2);
            this.splitContBottom.Name = "splitContBottom";
            // 
            // splitContBottom.Panel1
            // 
            this.splitContBottom.Panel1.Controls.Add(this.labelNumContacts);
            this.splitContBottom.Panel1.Controls.Add(this.label4);
            this.splitContBottom.Panel1.Controls.Add(this.lviewResult);
            // 
            // splitContBottom.Panel2
            // 
            this.splitContBottom.Panel2.Controls.Add(this.splitContBottomRight);
            this.splitContBottom.Size = new System.Drawing.Size(763, 323);
            this.splitContBottom.SplitterDistance = 351;
            this.splitContBottom.TabIndex = 1;
            // 
            // labelNumContacts
            // 
            this.labelNumContacts.AutoSize = true;
            this.labelNumContacts.Location = new System.Drawing.Point(46, 0);
            this.labelNumContacts.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.labelNumContacts.Name = "labelNumContacts";
            this.labelNumContacts.Size = new System.Drawing.Size(13, 13);
            this.labelNumContacts.TabIndex = 2;
            this.labelNumContacts.Text = "0";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(-2, 0);
            this.label4.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(52, 13);
            this.label4.TabIndex = 1;
            this.label4.Text = "Contacts:";
            // 
            // lviewResult
            // 
            this.lviewResult.AllowColumnReorder = true;
            this.lviewResult.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.lviewResult.FullRowSelect = true;
            this.lviewResult.GridLines = true;
            this.lviewResult.HideSelection = false;
            this.lviewResult.Location = new System.Drawing.Point(0, 16);
            this.lviewResult.Margin = new System.Windows.Forms.Padding(2);
            this.lviewResult.Name = "lviewResult";
            this.lviewResult.Size = new System.Drawing.Size(352, 308);
            this.lviewResult.TabIndex = 0;
            this.lviewResult.UseCompatibleStateImageBehavior = false;
            this.lviewResult.View = System.Windows.Forms.View.Details;
            this.lviewResult.VirtualMode = true;
            this.lviewResult.ColumnClick += new System.Windows.Forms.ColumnClickEventHandler(this.listResult_ColumnClick);
            this.lviewResult.RetrieveVirtualItem += new System.Windows.Forms.RetrieveVirtualItemEventHandler(this.listResult_RetrieveVirtualItem);
            this.lviewResult.SearchForVirtualItem += new System.Windows.Forms.SearchForVirtualItemEventHandler(this.listResult_SearchForVirtualItem);
            this.lviewResult.SelectedIndexChanged += new System.EventHandler(this.listResult_SelectedIndexChanged);
            // 
            // splitContBottomRight
            // 
            this.splitContBottomRight.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContBottomRight.Location = new System.Drawing.Point(0, 0);
            this.splitContBottomRight.Margin = new System.Windows.Forms.Padding(2);
            this.splitContBottomRight.Name = "splitContBottomRight";
            this.splitContBottomRight.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContBottomRight.Panel1
            // 
            this.splitContBottomRight.Panel1.Controls.Add(this.buttonChangeAttr);
            this.splitContBottomRight.Panel1.Controls.Add(this.buttonCopyAttr);
            this.splitContBottomRight.Panel1.Controls.Add(this.lviewDetails);
            this.splitContBottomRight.Panel1.Controls.Add(label2);
            // 
            // splitContBottomRight.Panel2
            // 
            this.splitContBottomRight.Panel2.Controls.Add(this.buttonClearPhoto);
            this.splitContBottomRight.Panel2.Controls.Add(this.buttonSelectPhoto);
            this.splitContBottomRight.Panel2.Controls.Add(this.personPhoto);
            this.splitContBottomRight.Panel2.Controls.Add(this.label3);
            this.splitContBottomRight.Size = new System.Drawing.Size(408, 323);
            this.splitContBottomRight.SplitterDistance = 130;
            this.splitContBottomRight.SplitterWidth = 3;
            this.splitContBottomRight.TabIndex = 0;
            // 
            // buttonChangeAttr
            // 
            this.buttonChangeAttr.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonChangeAttr.Enabled = false;
            this.buttonChangeAttr.Location = new System.Drawing.Point(334, 43);
            this.buttonChangeAttr.Margin = new System.Windows.Forms.Padding(2);
            this.buttonChangeAttr.Name = "buttonChangeAttr";
            this.buttonChangeAttr.Size = new System.Drawing.Size(75, 23);
            this.buttonChangeAttr.TabIndex = 3;
            this.buttonChangeAttr.Text = "Change...";
            this.buttonChangeAttr.UseVisualStyleBackColor = true;
            this.buttonChangeAttr.Click += new System.EventHandler(this.buttonChangeAttr_Click);
            // 
            // buttonCopyAttr
            // 
            this.buttonCopyAttr.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonCopyAttr.Enabled = false;
            this.buttonCopyAttr.Location = new System.Drawing.Point(334, 16);
            this.buttonCopyAttr.Margin = new System.Windows.Forms.Padding(2);
            this.buttonCopyAttr.Name = "buttonCopyAttr";
            this.buttonCopyAttr.Size = new System.Drawing.Size(75, 23);
            this.buttonCopyAttr.TabIndex = 2;
            this.buttonCopyAttr.Text = "Copy";
            this.buttonCopyAttr.UseVisualStyleBackColor = true;
            this.buttonCopyAttr.Click += new System.EventHandler(this.buttonCopyAttr_Click);
            // 
            // lviewDetails
            // 
            this.lviewDetails.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.lviewDetails.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.propertyName,
            this.propertyValue});
            this.lviewDetails.FullRowSelect = true;
            this.lviewDetails.GridLines = true;
            this.lviewDetails.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.Nonclickable;
            this.lviewDetails.HideSelection = false;
            this.lviewDetails.Location = new System.Drawing.Point(4, 16);
            this.lviewDetails.Margin = new System.Windows.Forms.Padding(2);
            this.lviewDetails.MultiSelect = false;
            this.lviewDetails.Name = "lviewDetails";
            this.lviewDetails.Size = new System.Drawing.Size(326, 105);
            this.lviewDetails.TabIndex = 1;
            this.lviewDetails.UseCompatibleStateImageBehavior = false;
            this.lviewDetails.View = System.Windows.Forms.View.Details;
            this.lviewDetails.SelectedIndexChanged += new System.EventHandler(this.lviewDetails_SelectedIndexChanged);
            this.lviewDetails.DoubleClick += new System.EventHandler(this.lviewDetails_DoubleClick);
            // 
            // propertyName
            // 
            this.propertyName.Text = "Attribute";
            this.propertyName.Width = 120;
            // 
            // propertyValue
            // 
            this.propertyValue.Text = "Value";
            this.propertyValue.Width = 120;
            // 
            // buttonClearPhoto
            // 
            this.buttonClearPhoto.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonClearPhoto.Enabled = false;
            this.buttonClearPhoto.Location = new System.Drawing.Point(334, 43);
            this.buttonClearPhoto.Margin = new System.Windows.Forms.Padding(2);
            this.buttonClearPhoto.Name = "buttonClearPhoto";
            this.buttonClearPhoto.Size = new System.Drawing.Size(75, 23);
            this.buttonClearPhoto.TabIndex = 3;
            this.buttonClearPhoto.Text = "Clear";
            this.buttonClearPhoto.UseVisualStyleBackColor = true;
            this.buttonClearPhoto.Click += new System.EventHandler(this.buttonClearPhoto_Click);
            // 
            // buttonSelectPhoto
            // 
            this.buttonSelectPhoto.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonSelectPhoto.Enabled = false;
            this.buttonSelectPhoto.Location = new System.Drawing.Point(334, 16);
            this.buttonSelectPhoto.Margin = new System.Windows.Forms.Padding(2);
            this.buttonSelectPhoto.Name = "buttonSelectPhoto";
            this.buttonSelectPhoto.Size = new System.Drawing.Size(75, 23);
            this.buttonSelectPhoto.TabIndex = 2;
            this.buttonSelectPhoto.Text = "Select...";
            this.buttonSelectPhoto.UseVisualStyleBackColor = true;
            this.buttonSelectPhoto.Click += new System.EventHandler(this.buttonSelectPhoto_Click);
            // 
            // personPhoto
            // 
            this.personPhoto.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.personPhoto.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.personPhoto.Location = new System.Drawing.Point(4, 16);
            this.personPhoto.Margin = new System.Windows.Forms.Padding(2);
            this.personPhoto.Name = "personPhoto";
            this.personPhoto.Size = new System.Drawing.Size(326, 190);
            this.personPhoto.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
            this.personPhoto.TabIndex = 1;
            this.personPhoto.TabStop = false;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Dock = System.Windows.Forms.DockStyle.Top;
            this.label3.Location = new System.Drawing.Point(0, 0);
            this.label3.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(38, 13);
            this.label3.TabIndex = 0;
            this.label3.Text = "Photo:";
            // 
            // buttonSettings
            // 
            this.buttonSettings.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.buttonSettings.Location = new System.Drawing.Point(10, 527);
            this.buttonSettings.Margin = new System.Windows.Forms.Padding(2);
            this.buttonSettings.Name = "buttonSettings";
            this.buttonSettings.Size = new System.Drawing.Size(75, 23);
            this.buttonSettings.TabIndex = 2;
            this.buttonSettings.Text = "Settings...";
            this.buttonSettings.UseVisualStyleBackColor = true;
            this.buttonSettings.Click += new System.EventHandler(this.buttonSettings_Click);
            // 
            // buttonAbout
            // 
            this.buttonAbout.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.buttonAbout.Location = new System.Drawing.Point(89, 527);
            this.buttonAbout.Margin = new System.Windows.Forms.Padding(2);
            this.buttonAbout.Name = "buttonAbout";
            this.buttonAbout.Size = new System.Drawing.Size(75, 23);
            this.buttonAbout.TabIndex = 3;
            this.buttonAbout.Text = "About";
            this.buttonAbout.UseVisualStyleBackColor = true;
            this.buttonAbout.Click += new System.EventHandler(this.buttonAbout_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(9, 7);
            this.label1.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(200, 13);
            this.label1.TabIndex = 4;
            this.label1.Text = "Find contacts matching these conditions:";
            // 
            // cboxFilterNames
            // 
            this.cboxFilterNames.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cboxFilterNames.FormattingEnabled = true;
            this.cboxFilterNames.Location = new System.Drawing.Point(10, 24);
            this.cboxFilterNames.Margin = new System.Windows.Forms.Padding(2);
            this.cboxFilterNames.Name = "cboxFilterNames";
            this.cboxFilterNames.Size = new System.Drawing.Size(129, 21);
            this.cboxFilterNames.TabIndex = 5;
            // 
            // cboxConditions
            // 
            this.cboxConditions.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cboxConditions.FormattingEnabled = true;
            this.cboxConditions.Location = new System.Drawing.Point(143, 24);
            this.cboxConditions.Margin = new System.Windows.Forms.Padding(2);
            this.cboxConditions.Name = "cboxConditions";
            this.cboxConditions.Size = new System.Drawing.Size(104, 21);
            this.cboxConditions.TabIndex = 6;
            // 
            // cboxFilterValues
            // 
            this.cboxFilterValues.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.cboxFilterValues.FormattingEnabled = true;
            this.cboxFilterValues.Location = new System.Drawing.Point(251, 24);
            this.cboxFilterValues.Margin = new System.Windows.Forms.Padding(2);
            this.cboxFilterValues.Name = "cboxFilterValues";
            this.cboxFilterValues.Size = new System.Drawing.Size(445, 21);
            this.cboxFilterValues.TabIndex = 7;
            this.cboxFilterValues.TextUpdate += new System.EventHandler(this.cboxFilterValues_TextUpdate);
            // 
            // buttonAddFilter
            // 
            this.buttonAddFilter.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonAddFilter.Enabled = false;
            this.buttonAddFilter.Location = new System.Drawing.Point(700, 22);
            this.buttonAddFilter.Margin = new System.Windows.Forms.Padding(2);
            this.buttonAddFilter.Name = "buttonAddFilter";
            this.buttonAddFilter.Size = new System.Drawing.Size(75, 23);
            this.buttonAddFilter.TabIndex = 8;
            this.buttonAddFilter.Text = "Add";
            this.buttonAddFilter.UseVisualStyleBackColor = true;
            this.buttonAddFilter.Click += new System.EventHandler(this.buttonAddFilter_Click);
            // 
            // lviewFilters
            // 
            this.lviewFilters.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.lviewFilters.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.colFilterName,
            this.colCondition,
            this.colFilterValue});
            this.lviewFilters.FullRowSelect = true;
            this.lviewFilters.GridLines = true;
            this.lviewFilters.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.Nonclickable;
            this.lviewFilters.HideSelection = false;
            this.lviewFilters.Location = new System.Drawing.Point(0, 2);
            this.lviewFilters.Margin = new System.Windows.Forms.Padding(2);
            this.lviewFilters.MultiSelect = false;
            this.lviewFilters.Name = "lviewFilters";
            this.lviewFilters.Size = new System.Drawing.Size(685, 106);
            this.lviewFilters.TabIndex = 9;
            this.lviewFilters.UseCompatibleStateImageBehavior = false;
            this.lviewFilters.View = System.Windows.Forms.View.Details;
            this.lviewFilters.SelectedIndexChanged += new System.EventHandler(this.lviewFilters_SelectedIndexChanged);
            this.lviewFilters.SizeChanged += new System.EventHandler(this.lviewFilters_SizeChanged);
            // 
            // colFilterName
            // 
            this.colFilterName.Text = "Filter";
            this.colFilterName.Width = 120;
            // 
            // colCondition
            // 
            this.colCondition.Text = "Condition";
            this.colCondition.Width = 120;
            // 
            // colFilterValue
            // 
            this.colFilterValue.Text = "Value";
            this.colFilterValue.Width = 200;
            // 
            // buttonRemoveFilter
            // 
            this.buttonRemoveFilter.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonRemoveFilter.Enabled = false;
            this.buttonRemoveFilter.Location = new System.Drawing.Point(689, 2);
            this.buttonRemoveFilter.Margin = new System.Windows.Forms.Padding(2);
            this.buttonRemoveFilter.Name = "buttonRemoveFilter";
            this.buttonRemoveFilter.Size = new System.Drawing.Size(75, 23);
            this.buttonRemoveFilter.TabIndex = 10;
            this.buttonRemoveFilter.Text = "Remove";
            this.buttonRemoveFilter.UseVisualStyleBackColor = true;
            this.buttonRemoveFilter.Click += new System.EventHandler(this.buttonRemoveFilter_Click);
            // 
            // buttonFind
            // 
            this.buttonFind.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonFind.Location = new System.Drawing.Point(689, 107);
            this.buttonFind.Margin = new System.Windows.Forms.Padding(2);
            this.buttonFind.Name = "buttonFind";
            this.buttonFind.Size = new System.Drawing.Size(75, 23);
            this.buttonFind.TabIndex = 11;
            this.buttonFind.Text = "Find";
            this.buttonFind.UseVisualStyleBackColor = true;
            this.buttonFind.Click += new System.EventHandler(this.buttonFind_Click);
            // 
            // splitCont
            // 
            this.splitCont.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.splitCont.Location = new System.Drawing.Point(11, 48);
            this.splitCont.Margin = new System.Windows.Forms.Padding(2);
            this.splitCont.Name = "splitCont";
            this.splitCont.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitCont.Panel1
            // 
            this.splitCont.Panel1.Controls.Add(this.rbuttonAnyFilter);
            this.splitCont.Panel1.Controls.Add(this.rbuttonAllFilters);
            this.splitCont.Panel1.Controls.Add(this.lviewFilters);
            this.splitCont.Panel1.Controls.Add(this.buttonFind);
            this.splitCont.Panel1.Controls.Add(this.buttonRemoveFilter);
            this.splitCont.Panel1MinSize = 100;
            // 
            // splitCont.Panel2
            // 
            this.splitCont.Panel2.Controls.Add(this.splitContBottom);
            this.splitCont.Panel2MinSize = 200;
            this.splitCont.Size = new System.Drawing.Size(763, 463);
            this.splitCont.SplitterDistance = 137;
            this.splitCont.SplitterWidth = 3;
            this.splitCont.TabIndex = 12;
            // 
            // rbuttonAnyFilter
            // 
            this.rbuttonAnyFilter.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.rbuttonAnyFilter.AutoSize = true;
            this.rbuttonAnyFilter.Location = new System.Drawing.Point(203, 113);
            this.rbuttonAnyFilter.Margin = new System.Windows.Forms.Padding(2);
            this.rbuttonAnyFilter.Name = "rbuttonAnyFilter";
            this.rbuttonAnyFilter.Size = new System.Drawing.Size(231, 17);
            this.rbuttonAnyFilter.TabIndex = 13;
            this.rbuttonAnyFilter.Text = "At least one of the conditions should be met";
            this.rbuttonAnyFilter.UseVisualStyleBackColor = true;
            // 
            // rbuttonAllFilters
            // 
            this.rbuttonAllFilters.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.rbuttonAllFilters.AutoSize = true;
            this.rbuttonAllFilters.Checked = true;
            this.rbuttonAllFilters.Location = new System.Drawing.Point(3, 114);
            this.rbuttonAllFilters.Margin = new System.Windows.Forms.Padding(2);
            this.rbuttonAllFilters.Name = "rbuttonAllFilters";
            this.rbuttonAllFilters.Size = new System.Drawing.Size(174, 17);
            this.rbuttonAllFilters.TabIndex = 12;
            this.rbuttonAllFilters.TabStop = true;
            this.rbuttonAllFilters.Text = "All the conditions should be met";
            this.rbuttonAllFilters.UseVisualStyleBackColor = true;
            // 
            // selectPhotoDlg
            // 
            this.selectPhotoDlg.Filter = "*.jpeg files|*.jpg;*.jpeg";
            this.selectPhotoDlg.InitialDirectory = global::WfAdBook.Properties.Settings.Default.SelectPhotoDlgInitDir;
            this.selectPhotoDlg.ShowReadOnly = true;
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(784, 561);
            this.Controls.Add(this.splitCont);
            this.Controls.Add(this.buttonAddFilter);
            this.Controls.Add(this.cboxFilterValues);
            this.Controls.Add(this.cboxConditions);
            this.Controls.Add(this.cboxFilterNames);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.buttonAbout);
            this.Controls.Add(this.buttonSettings);
            this.Controls.Add(this.buttonExit);
            this.DataBindings.Add(new System.Windows.Forms.Binding("Location", global::WfAdBook.Properties.Settings.Default, "MainFormLocation", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Location = global::WfAdBook.Properties.Settings.Default.MainFormLocation;
            this.Margin = new System.Windows.Forms.Padding(2);
            this.MinimumSize = new System.Drawing.Size(800, 600);
            this.Name = "MainForm";
            this.Text = "Active Directory Contact Book";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.MainForm_FormClosed);
            this.Shown += new System.EventHandler(this.MainForm_Shown);
            this.splitContBottom.Panel1.ResumeLayout(false);
            this.splitContBottom.Panel1.PerformLayout();
            this.splitContBottom.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContBottom)).EndInit();
            this.splitContBottom.ResumeLayout(false);
            this.splitContBottomRight.Panel1.ResumeLayout(false);
            this.splitContBottomRight.Panel1.PerformLayout();
            this.splitContBottomRight.Panel2.ResumeLayout(false);
            this.splitContBottomRight.Panel2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContBottomRight)).EndInit();
            this.splitContBottomRight.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.personPhoto)).EndInit();
            this.splitCont.Panel1.ResumeLayout(false);
            this.splitCont.Panel1.PerformLayout();
            this.splitCont.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitCont)).EndInit();
            this.splitCont.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button buttonExit;
        private System.Windows.Forms.SplitContainer splitContBottom;
        private System.Windows.Forms.Button buttonSettings;
        private System.Windows.Forms.Button buttonAbout;
        private System.Windows.Forms.ListView lviewResult;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ComboBox cboxFilterNames;
        private System.Windows.Forms.ComboBox cboxConditions;
        private System.Windows.Forms.ComboBox cboxFilterValues;
        private System.Windows.Forms.Button buttonAddFilter;
        private System.Windows.Forms.ListView lviewFilters;
        private System.Windows.Forms.Button buttonRemoveFilter;
        private System.Windows.Forms.Button buttonFind;
        private System.Windows.Forms.ColumnHeader colFilterName;
        private System.Windows.Forms.ColumnHeader colCondition;
        private System.Windows.Forms.ColumnHeader colFilterValue;
        private System.Windows.Forms.SplitContainer splitCont;
        private System.Windows.Forms.SplitContainer splitContBottomRight;
        private System.Windows.Forms.ListView lviewDetails;
        private System.Windows.Forms.PictureBox personPhoto;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.OpenFileDialog selectPhotoDlg;
        private System.Windows.Forms.Button buttonChangeAttr;
        private System.Windows.Forms.Button buttonCopyAttr;
        private System.Windows.Forms.Button buttonClearPhoto;
        private System.Windows.Forms.Button buttonSelectPhoto;
        private System.Windows.Forms.Label labelNumContacts;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.RadioButton rbuttonAnyFilter;
        private System.Windows.Forms.RadioButton rbuttonAllFilters;
        private System.Windows.Forms.ColumnHeader propertyName;
        private System.Windows.Forms.ColumnHeader propertyValue;
    }
}

