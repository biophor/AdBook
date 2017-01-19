﻿// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using adbookcli;
using System.Diagnostics;
using System.Collections;
using System.IO;

namespace WfAdBook
{
    public partial class MainForm : Form //-V3073
    {        
        private int _sortCol = -1;
        private SortOrder _sortOrder = SortOrder.None;
        private Stopwatch _timer = new Stopwatch();
        private string[] _columnIndexToAttrName;
        private List<ListViewItem> _personListItems = new List<ListViewItem>();        
        private AdSearchTask _adSearchTask = null;
        public MainForm()
        {
            InitializeComponent();
            InitFilterList();
            InitFilterNamesList();
            InitLdapMatchingRules();            
            InitPersonList();
            InitPersonDetails();
            ApplyFontSettings();
            Disposed += MainForm_Disposed;
        }        
        private void InitFilterList()
        {
            ListCtrlHelper.EnableTooltipsForListCtrl(lviewFilters.Handle);
            RestoreFilterListColumnsSizes();
        }
        private void InitPersonDetails()
        {            
            foreach (var attrId in AdAttributes.AttrIds) {
                var attr = AdAttributes.Get(attrId);
                if (attr.IsString && !attr.IsReadOnly) {
                    var listViewItem = lviewDetails.Items.Add(attr.DisplayName);
                    listViewItem.SubItems.Add("");  // attr value                    
                    listViewItem.Tag = attr;
                }
            }            
            ListCtrlHelper.EnableTooltipsForListCtrl(lviewDetails.Handle);
            RestorePersonDetailsColumnsSizes();            
        }
        private void RestorePersonDetailsColumnsSizes()
        {
            RestoreListViewColumnsSizes(lviewDetails,
                Properties.Settings.Default.MainFormPersonDetailsColumnSizes);
        }
        private void SavePersonDetailsColumnsSizes()
        {
            Properties.Settings.Default.MainFormPersonDetailsColumnSizes = 
                ListViewColumnsSizesToString(lviewDetails);
        }
        private void InitPersonList()
        {
            // create columns
            _columnIndexToAttrName = new string[AdAttributes.AttrIds.Count];
            foreach (var attrId in AdAttributes.AttrIds) {
                var attr = AdAttributes.Get(attrId);
                if (attr.IsString && !attr.IsReadOnly) {
                    var column = lviewResult.Columns.Add(attr.DisplayName);
                    _columnIndexToAttrName[column.Index] = attr.LdapName;
                }
            }
            ListCtrlHelper.EnableTooltipsForListCtrl(lviewResult.Handle);
            RestorePersonListColumnsSizes();
            RestorePersonListColumnsOrder();
        }
        private void buttonExit_Click(object sender, EventArgs e)
        {
            Close();
        }
        private void ApplyFontSettings()
        {
            Action<ListView, Font> setupFont = (ListView lv, Font newFont) => {
                if (newFont == null) {
                    return;
                }
                lv.Font = new Font(newFont, newFont.Style);                
            };
            FontSettings fontSettings = new FontSettings();
            setupFont(lviewResult, fontSettings.ContactListFont);
            setupFont(lviewDetails, fontSettings.ContactDetailsFont);
            setupFont(lviewFilters, fontSettings.FilterListFont);
        }
        private void buttonSettings_Click(object sender, EventArgs e)
        {            
            using (var settingsForm = new SettingsForm()) {
                if (settingsForm.ShowDialog() == DialogResult.OK) {
                    ApplyFontSettings();
                }
            }            
        }
        private void buttonAbout_Click(object sender, EventArgs e)
        {
            using (var about = new AboutForm()) {
                about.ShowDialog();
            }

        }        
        private void _adSearcher_SearchStoppedEvent(object sender, EventArgs e)
        {
            BeginInvoke(new Action(RefreshFormUiAfterSearchIsStopped));
        }
        private void _adSearcher_SearchStartedEvent(object sender, EventArgs e)
        {            
            BeginInvoke(new Action(RefreshFormUiAfterSearchIsStarted));
        }

        private void _adSearcher_PersonFoundEvent(object sender, PersonFoundEventArgs args)
        {
            // this method is called from non-ui thread
            lock (_personListItems) {
                var lvi = CreateListViewItem(args.Person);
                _personListItems.Add(lvi);
            }
            lock (_timer) {
                if (_timer.ElapsedMilliseconds < 500) {
                    return;
                }
                _timer.Restart();
            }
            BeginInvoke(new Action(() => { RefreshFormUiAfterPersonFound(); }));
        }

        private void RefreshFormUiAfterPersonFound()
        {
            Debug.Assert(lviewResult.InvokeRequired == false);
            lock (_personListItems) {
                lviewResult.VirtualListSize = _personListItems.Count;
            }
            labelNumContacts.Text = Convert.ToString(lviewResult.VirtualListSize); 
        }
        private ListViewItem CreateListViewItem(AdPerson adp)
        {
            if (adp == null) {
                throw new ArgumentNullException("adp");
            }
            string attrName = _columnIndexToAttrName[0];
            ListViewItem lvi = new ListViewItem();
            lvi.SubItems[0].Text = adp.get_StringAttr(attrName);
            for (int i = 1; i < lviewResult.Columns.Count; ++i) {
                attrName = _columnIndexToAttrName[i];
                lvi.SubItems.Add(adp.get_StringAttr(attrName));
            }
            lvi.SubItems.Add("invisible column to cache a photo");
            lvi.Tag = adp;
            return lvi;
        }        
        private void RefreshFormUiAfterSearchIsStopped()
        {
            lock (_timer) {
                _timer.Stop();
            }
            try {
                if (_adSearchTask!= null) {
                    _adSearchTask.Wait();
                    _adSearchTask.Dispose();
                    _adSearchTask = null;
                }
            }
            catch (Exception exc) {
                MessageBox.Show(this, exc.Message, Properties.Resources.ErrorMsgBoxTitle,
                    MessageBoxButtons.OK, MessageBoxIcon.Error
                    );
            }
            Debug.Assert(buttonFind.InvokeRequired == false);
            buttonFind.Text = Properties.Resources.FindButtonTextBeforeSearch;

            if (cboxFilterValues.Text.Length > 0) {
                buttonAddFilter.Enabled = true;
            }
            if (lviewFilters.SelectedItems.Count > 0) {
                lviewFilters.Enabled = true;
            }
            RefreshFormUiAfterPersonFound();
        }        
        private void RefreshFormUiAfterSearchIsStarted()
        {
            Debug.Assert(buttonFind.InvokeRequired == false);
            buttonAddFilter.Enabled = false;
            buttonRemoveFilter.Enabled = false;
            buttonSelectPhoto.Enabled = false;
            buttonCopyAttr.Enabled = false;
            buttonClearPhoto.Enabled = false;            
            buttonFind.Text = Properties.Resources.FindButtonTextDuringSearch;
            lviewResult.VirtualListSize = 0;
            lock (_personListItems) {
                _personListItems.Clear();
            }
            lock (_timer) {
                _timer.Start();
            }
        }        
        private void InitLdapMatchingRules()
        {
            foreach (var mr in LdapMatchingRules.Instance) {
                cboxConditions.Items.Add(mr);
            }
            cboxConditions.SelectedIndex = 0;
        }
        private void InitFilterNamesList()
        { 
            foreach (var cf in CompositeFilters.Instance) {
                cboxFilterNames.Items.Add(cf);
            }
            foreach (var attrId in AdAttributes.AttrIds) {
                var attr = AdAttributes.Get(attrId);
                if (attr.IsString && !attr.IsReadOnly) {
                    cboxFilterNames.Items.Add(attr);
                }                
            }            
            cboxFilterNames.SelectedIndex = 0;
        }
        private void CreateFilter()
        {
            var item = new ListViewItem(cboxFilterNames.SelectedItem.ToString());
            item.Tag = cboxFilterNames.SelectedItem;
            var matchingRuleItem = item.SubItems.Add(cboxConditions.SelectedItem.ToString());
            matchingRuleItem.Tag = cboxConditions.SelectedItem;
            var filterValue = item.SubItems.Add(cboxFilterValues.Text);
            lviewFilters.Items.Add(item);
        }
        private void buttonAddFilter_Click(object sender, EventArgs e)
        {
            CreateFilter();
        }
        private void RemoveFilter()
        {
            var selectedItems = lviewFilters.SelectedItems;
            while (selectedItems.Count != 0) {
                lviewFilters.Items.Remove(selectedItems[0]);
            }
        }
        private void buttonRemoveFilter_Click(object sender, EventArgs e)
        {
            RemoveFilter();
        }
        private LdapRequest ConstructLdapRequest()
        {
            LdapRequest lr = new adbookcli.LdapRequest();
            foreach (ListViewItem filterItem in lviewFilters.Items) {
                LdapMatchingRule lmr = (LdapMatchingRule)filterItem.SubItems[1].Tag;
                string filterValue = filterItem.SubItems[2].Text;
                AdAttribute adAttr = filterItem.Tag as AdAttribute;
                if (adAttr != null) {
                    lr.AddRule(adAttr.Id, lmr.Id, filterValue);                    
                    continue;
                }
                CompositeFilter cf = filterItem.Tag as CompositeFilter;
                if (cf != null) {
                    if (cf.Id == CompositeFilterId.AnyAttribute) {
                        foreach (var attrId in AdAttributes.AttrIds) {
                            var attr = AdAttributes.Get(attrId);
                            if (attr.IsString && !attr.IsReadOnly) {
                                lr.AddRule(attrId, lmr.Id, filterValue);                                
                            }
                        }
                        lr.AddOR();
                    }
                    continue;
                }
                throw new InvalidOperationException("unknown filter type");
            }
            if (lviewFilters.Items.Count > 0) {
                if (rbuttonAllFilters.Checked) {
                    lr.AddAND();
                }
                if (rbuttonAnyFilter.Checked) {
                    lr.AddOR();
                }
            }
            lr.AddRule("objectCategory", LdapRequest.MathingRule.ExactMatch, "person");
            if (lviewFilters.Items.Count > 0) {
                lr.AddAND();
            }
            return lr;
        }
        private async void ToggleSearch()
        {
            if (_adSearchTask != null) {
                if (_adSearchTask.Status == System.Threading.Tasks.TaskStatus.Running) {
                    _adSearchTask.Cancel();
                    return;
                }
                return;
            }            
            try {                
                DisposePersonItemsInAdPersonList();
                using (var cs = new ConnectionSettings()) {
                    using (var request = ConstructLdapRequest()) {
                        using (_adSearchTask = new AdSearchTask(new AdSearchTask.Arguments(request, cs))) {
                            _adSearchTask.PersonFoundEvent += _adSearcher_PersonFoundEvent;
                            _adSearchTask.Start();
                            RefreshFormUiAfterSearchIsStarted();
                            await _adSearchTask;
                        }                            
                    }
                }
                
            }
            catch (Exception exc) {
                MessageBox.Show(exc.Message, Properties.Resources.ErrorMsgBoxTitle,
                    MessageBoxButtons.OK, MessageBoxIcon.Error
                    );
            }
            finally {
                _adSearchTask = null;
                RefreshFormUiAfterSearchIsStopped();
            }            
        }
        private bool CheckConnection()
        {
            using (var cs = new ConnectionSettings()) {
                using (var adc = new AdConnector(cs)) {
                    try {
                        Cursor = Cursors.WaitCursor;
                        adc.Connect();
                    }
                    catch (Exception exc) {
                        MessageBox.Show(exc.Message, Properties.Resources.ErrorMsgBoxTitle,
                            MessageBoxButtons.OK, MessageBoxIcon.Error
                            );
                        return false;
                    }
                    finally {
                        Cursor = Cursors.Default;
                    }
                }
            }
            return true;
        }
        private void buttonFind_Click(object sender, EventArgs e)
        {
            if (CheckConnection()) {
                ToggleSearch();
            }
        }
        private void buttonCopyAttr_Click(object sender, EventArgs e)
        {
            if (lviewDetails.SelectedItems.Count == 0) {
                return;
            }
            Clipboard.SetText(lviewDetails.SelectedItems[0].SubItems[1].Text);
        }
        private void ChangeSelectedAttribute()
        {
            if (lviewDetails.SelectedItems.Count == 0) {
                return;
            }
            var attr = (AdAttribute)lviewDetails.SelectedItems[0].Tag;
            var adp = (AdPerson)lviewDetails.Tag;
            var currentValue = lviewDetails.SelectedItems[0].SubItems[1].Text;
            using (ModifyAttrForm f = new ModifyAttrForm(adp, attr, currentValue)) {
                if (f.ShowDialog() == DialogResult.OK) {
                    lviewDetails.SelectedItems[0].SubItems[1].Text = adp.get_StringAttr(attr.LdapName);
                    RefreshSelectedPersonDetails();
                    RefreshSelectedPersonItem();
                    UpdatePersonDetailsButtonStates();
                }
            }
        }
        private void buttonChangeAttr_Click(object sender, EventArgs e)
        {
            ChangeSelectedAttribute();
        }
        private void buttonSelectPhoto_Click(object sender, EventArgs e)
        {            
            if (selectPhotoDlg.ShowDialog() != DialogResult.OK) {
                return;
            }
            var stream = selectPhotoDlg.OpenFile();
            if (stream == null) {
                MessageBox.Show(this, Properties.Resources.FailedToReadTheFileMsgBoxText,
                        Properties.Resources.ErrorMsgBoxTitle,
                        MessageBoxButtons.OK, MessageBoxIcon.Error
                        );
                return;
            }
            byte[] buf = null;
            using (stream) {
                if (stream.Length == 0) {
                    MessageBox.Show(this, Properties.Resources.FileIsEmptyMsgBoxText,
                        Properties.Resources.ErrorMsgBoxTitle,
                        MessageBoxButtons.OK, MessageBoxIcon.Error
                        );
                    return;
                }
                if (stream.Length > AdAttributes.Get(AttrId.ThumbnailPhoto).MaxLen) {
                    MessageBox.Show(this, Properties.Resources.PhotoFileIsTooBigMsgBoxText,
                        Properties.Resources.ErrorMsgBoxTitle,
                        MessageBoxButtons.OK, MessageBoxIcon.Error
                        );
                    return;
                }
                buf = new byte[stream.Length];
                if (stream.Read(buf, 0, buf.Length) != stream.Length) {
                    MessageBox.Show(this, Properties.Resources.FailedToReadTheFileMsgBoxText,
                        Properties.Resources.ErrorMsgBoxTitle,
                        MessageBoxButtons.OK, MessageBoxIcon.Error
                        );
                    return;
                }
            }
            var adp = (AdPerson)lviewDetails.Tag;
            using (var cs = new ConnectionSettings()) {             
                using (var adc = new AdConnector(cs, adp.Dn)) {
                    try {
                        Cursor = Cursors.WaitCursor;
                        adc.Connect();
                        string attrName = AdAttributes.Get(AttrId.ThumbnailPhoto).LdapName;
                        adc.UploadBinaryAttr(attrName, buf);
                        adp.set_BinaryAttr(attrName, buf);
                        SetCachedPhotoForSelectedPerson(null);
                    }
                    catch (Exception exc) {
                        Cursor = Cursors.Default;
                        MessageBox.Show(this, exc.Message, Properties.Resources.ErrorMsgBoxTitle,
                            MessageBoxButtons.OK, MessageBoxIcon.Error
                            );
                    }
                    finally {
                        Cursor = Cursors.Default;
                    }
                }
            }
            RefreshSelectedPersonDetails();
            UpdatePersonDetailsButtonStates();
        }
        private void buttonClearPhoto_Click(object sender, EventArgs e)
        {
            var adp = (AdPerson)lviewDetails.Tag;
            using (var cs = new ConnectionSettings()) {
                using (var adc = new AdConnector(cs, adp.Dn)) {
                    try {
                        Cursor = Cursors.WaitCursor;
                        adc.Connect();
                        string attrName = AdAttributes.Get(AttrId.ThumbnailPhoto).LdapName;
                        var emptyBuf = new byte[0];
                        adc.UploadBinaryAttr(attrName, emptyBuf);
                        adp.set_BinaryAttr(attrName, emptyBuf);
                        SetCachedPhotoForSelectedPerson(null);
                    }
                    catch (Exception exc) {
                        Cursor = Cursors.Default;
                        MessageBox.Show(this, exc.Message, Properties.Resources.ErrorMsgBoxTitle,
                            MessageBoxButtons.OK, MessageBoxIcon.Error
                            );
                    }
                    finally {
                        Cursor = Cursors.Default;
                    }
                }
            }
            RefreshSelectedPersonDetails();
            UpdatePersonDetailsButtonStates();
        }
        private void SetCachedPhotoForSelectedPerson(Image image)
        {
            if (lviewResult.SelectedIndices.Count == 0) {
                return;
            }
            int itemIndex = lviewResult.SelectedIndices[0];
            int lastSubItemIndex = _personListItems[itemIndex].SubItems.Count - 1;
            lock (_personListItems) {
                _personListItems[itemIndex].SubItems[lastSubItemIndex].Tag = image;
            }
        }
        private Image GetCachedPhotoForSelectedPerson()
        {
            if (lviewResult.SelectedIndices.Count == 0) {
                return null;
            }
            int itemIndex = lviewResult.SelectedIndices[0];
            int lastSubItemIndex = _personListItems[itemIndex].SubItems.Count - 1;
            lock (_personListItems) {
                return (Image)_personListItems[itemIndex].SubItems[lastSubItemIndex].Tag;
            }
        }
        private void RefreshSelectedPersonItem()
        {
            if (lviewResult.SelectedIndices.Count == 0) {
                return;
            }
            int itemIndex = lviewResult.SelectedIndices[0];
            AdPerson adp = null;
            lock (_personListItems) {
                adp = (AdPerson)_personListItems[itemIndex].Tag;
            }
            bool redrawItem = false;
            for (int i = 0; i < lviewResult.Columns.Count; ++i) {
                string attrName = _columnIndexToAttrName[i];
                string lvmText = lviewResult.Items[itemIndex].SubItems[i].Text;
                string adpText = adp.get_StringAttr(attrName);
                if (lvmText != adpText) {
                    lviewResult.Items[itemIndex].SubItems[i].Text = adpText;
                    redrawItem = true;
                }
            }
            if (redrawItem) {
                lviewResult.RedrawItems(itemIndex, itemIndex, false);
            }
        }
        private void RefreshSelectedPersonDetails()
        {            
            if (lviewResult.SelectedIndices.Count == 0) {
                return;
            }
            int itemIndex = lviewResult.SelectedIndices[0];
            AdPerson adp = null;
            lock (_personListItems) {
                adp = (AdPerson)_personListItems[itemIndex].Tag;
            }
            bool redrawItem = false;
            for (int i = 0; i < lviewResult.Columns.Count; ++i) {
                string attrName = _columnIndexToAttrName[i];
                string lvmText = lviewResult.Items[itemIndex].SubItems[i].Text;
                string adpText = adp.get_StringAttr(attrName);
                if (lvmText != adpText) {
                    lviewResult.Items[itemIndex].SubItems[i].Text = adpText;
                    redrawItem = true;
                }
            }
            if (redrawItem) {
                lviewResult.RedrawItems(itemIndex, itemIndex, false);                
            }
            foreach (ListViewItem lvi in lviewDetails.Items) {
                AdAttribute ada = (AdAttribute)lvi.Tag;
                string attrValue = adp.get_StringAttr(ada.LdapName);
                lvi.SubItems[1].Text = attrValue;
            }
            lviewDetails.Tag = adp;         
            int lastSubItemIndex = _personListItems[itemIndex].SubItems.Count - 1;
            lock (_personListItems) {
                Image cachedPhoto = GetCachedPhotoForSelectedPerson();
                if (cachedPhoto != null) {
                    personPhoto.Image = cachedPhoto;
                    return;
                }
                byte[] imageBytes = adp.get_BinaryAttr(
                    AdAttributes.Get(adbookcli.AttrId.ThumbnailPhoto).LdapName
                        );
                if (imageBytes.Length > 0) {
                    personPhoto.Image = Image.FromStream(new MemoryStream(imageBytes));                    
                    SetCachedPhotoForSelectedPerson(personPhoto.Image);
                }
                else {
                    personPhoto.Image = null;
                }
            }
        }
        private void listResult_SelectedIndexChanged(object sender, EventArgs e)
        {            
            RefreshSelectedPersonDetails();
            UpdatePersonDetailsButtonStates();
        }
        private void listResult_ColumnClick(object sender, ColumnClickEventArgs e)
        {            
            if (e.Column == _sortCol) {
                if (_sortOrder == SortOrder.Ascending || _sortOrder == SortOrder.None) {
                    _sortOrder = SortOrder.Descending;
                }
                else {
                    _sortOrder = SortOrder.Ascending;
                }
            }            
            var sorter = new ListViewItemComparer(e.Column, _sortOrder);
            lock (_personListItems) {
                _personListItems.Sort(sorter);
            }
            _sortCol = e.Column;
            lviewResult.Invalidate();
        }
        private void UpdatePersonDetailsButtonStates()
        {
            buttonCopyAttr.Enabled = false;
            buttonChangeAttr.Enabled = false;
            if (lviewDetails.SelectedIndices.Count == 0) {
                return;
            }
            if (lviewDetails.Tag == null) {
                return;
            }
            AdPerson adp = (AdPerson)lviewDetails.Tag;
            int selAttrIndex = lviewDetails.SelectedIndices[0];
            AdAttribute attr = (AdAttribute)lviewDetails.Items[selAttrIndex].Tag;
            string attrValue = lviewDetails.Items[selAttrIndex].SubItems[1].Text;
            buttonCopyAttr.Enabled = !string.IsNullOrEmpty(attrValue);
            buttonChangeAttr.Enabled = adp.IsAttributeWritable(attr.Id);
            bool canChangePhoto = adp.IsAttributeWritable(AttrId.ThumbnailPhoto);
            buttonClearPhoto.Enabled = canChangePhoto;
            buttonSelectPhoto.Enabled = canChangePhoto;
        }
        private void lviewDetails_SelectedIndexChanged(object sender, EventArgs e)
        {
            UpdatePersonDetailsButtonStates();
        }        
        private void SavePersonListColumnsSizes()
        {            
            Properties.Settings.Default.MainFormPersonListColumnSizes = ListViewColumnsSizesToString(lviewResult);
        }
        private void SaveFilterListColumnsSizes()
        {
            Properties.Settings.Default.MainFormFilterListColumnSizes = ListViewColumnsSizesToString(lviewFilters);
        }
        private string ListViewColumnsSizesToString(ListView lview)
        {
            StringBuilder sb = new StringBuilder();
            for (int i = 0; i < lview.Columns.Count; ++i) {
                sb.AppendFormat("{0} ", lview.Columns[i].Width);
            }
            return sb.ToString().TrimEnd();
        }
        private void SavePersonListColumnsOrder()
        {
            StringBuilder sb = new StringBuilder();
            for (int i = 0; i < lviewResult.Columns.Count; ++i) {
                sb.AppendFormat("{0} ", lviewResult.Columns[i].DisplayIndex);
            }
            Properties.Settings.Default.MainFormPersonListColumnOrder = sb.ToString().TrimEnd();
        }
        private void MainForm_Disposed(object sender, EventArgs e)
        {
            DisposePersonItemsInAdPersonList();
            _adSearchTask?.Dispose();
            _adSearchTask = null;
        }
        private void MainForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            Properties.Settings.Default.MainFormWindowState = WindowState;
            if (WindowState == FormWindowState.Normal) {
                Properties.Settings.Default.MainFormSize = new Size(Width, Height);                
            }
            Properties.Settings.Default.MainFormSplitterDistance = splitCont.SplitterDistance;
            Properties.Settings.Default.MainFormBottomSplitterDistance =
                splitContBottom.SplitterDistance;
            Properties.Settings.Default.MainFormBottomRightSplitterDistance =
                splitContBottomRight.SplitterDistance;
            SavePersonListColumnsSizes();
            SavePersonListColumnsOrder();
            SavePersonDetailsColumnsSizes();
            SaveFilterListColumnsSizes();
            Properties.Settings.Default.Save();
            SaveAdPersonList();            
        }
        private void SaveAdPersonList()
        {
            using (adbookcli.AdPersonListKeeper adplk = new adbookcli.AdPersonListKeeper()) {
                lock (_personListItems) {
                    foreach (var adp in _personListItems) {
                        adplk.Add((AdPerson)adp.Tag);
                    }
                }
                adplk.Save();
            }
        }
        private void DisposePersonItemsInAdPersonList()
        {
            lock (_personListItems) {
                foreach (var adp in _personListItems) {
                    (adp.Tag as AdPerson).Dispose();
                    adp.Tag = null;
                }
            }            
        }
        private void MainForm_Shown(object sender, EventArgs e)
        {
            WindowState = Properties.Settings.Default.MainFormWindowState;
            if (WindowState == FormWindowState.Normal) {
                Width = Properties.Settings.Default.MainFormSize.Width;
                Height = Properties.Settings.Default.MainFormSize.Height;
            }
            splitCont.SplitterDistance = Properties.Settings.Default.MainFormSplitterDistance;
            splitContBottom.SplitterDistance = Properties.Settings.Default.MainFormBottomSplitterDistance;
            splitContBottomRight.SplitterDistance =
                Properties.Settings.Default.MainFormBottomRightSplitterDistance;
            RestorePersonListColumnsSizes();
            LoadAdBookItems();
        }
        private void LoadAdBookItems()
        {
            using (AdPersonListKeeper adplk = new AdPersonListKeeper()) {
                adplk.Load();
                lock (_personListItems) {
                    foreach (var adp in adplk) {
                        var tvi = CreateListViewItem(adp);
                        _personListItems.Add(tvi);
                    }
                    lviewResult.VirtualListSize = _personListItems.Count;
                }
                labelNumContacts.Text = Convert.ToString(lviewResult.VirtualListSize);
            }                
        }
        private void RestoreFilterListColumnsSizes()
        {
            RestoreListViewColumnsSizes(lviewFilters,
                Properties.Settings.Default.MainFormFilterListColumnSizes);
        }
        private void RestorePersonListColumnsSizes()
        {
            RestoreListViewColumnsSizes(lviewResult, 
                Properties.Settings.Default.MainFormPersonListColumnSizes);
        }
        private void RestoreListViewColumnsSizes(ListView lview, string colSizesFromSettings)
        {            
            if (!string.IsNullOrWhiteSpace(colSizesFromSettings)) {
                var v = (colSizesFromSettings.Split(new char[] { ' ' }));
                for (int i = 0; i < v.Length; ++i) {
                    int colWidth = 0;
                    if (int.TryParse(v[i], out colWidth)) {
                        lview.Columns[i].Width = colWidth;
                    }
                    else {
                        break;
                    }
                }
            }
        }
        private void RestorePersonListColumnsOrder()
        {
            string s = Properties.Settings.Default.MainFormPersonListColumnOrder;
            if (!string.IsNullOrWhiteSpace(s)) {
                var v = (s.Split(new char[] { ' ' }));
                for (int i = 0; i < v.Length; ++i) {
                    int colDisplayIndex = 0;
                    if (int.TryParse(v[i], out colDisplayIndex)) {                        
                        lviewResult.Columns[i].DisplayIndex = colDisplayIndex;
                    }
                    else {
                        break;
                    }
                }
            }
        }
        private void listResult_RetrieveVirtualItem(object sender, RetrieveVirtualItemEventArgs e)
        {
            lock (_personListItems) {
                e.Item = _personListItems[e.ItemIndex];
            }
        }
        private void listResult_SearchForVirtualItem(object sender, SearchForVirtualItemEventArgs e)
        {
            
        }
        private void lviewFilters_SizeChanged(object sender, EventArgs e)
        {

        }
        private void cboxFilterValues_TextUpdate(object sender, EventArgs e)
        {            
            buttonAddFilter.Enabled = !string.IsNullOrEmpty(cboxFilterValues.Text);
        }
        private void lviewFilters_SelectedIndexChanged(object sender, EventArgs e)
        {
            buttonRemoveFilter.Enabled = lviewFilters.SelectedItems.Count != 0;
        }
        class ListViewItemComparer : IComparer<ListViewItem>, IComparer
        {
            private SortOrder _sortOrder;
            private int _col;
            public ListViewItemComparer(int column, SortOrder sortOrder = SortOrder.Ascending)
            {
                _col = column;
                _sortOrder = sortOrder;
            }
            int IComparer.Compare(object x, object y)
            {
                return Compare((ListViewItem)x, (ListViewItem)y);
            }
            public int Compare(ListViewItem x, ListViewItem y)
            {
                if (_sortOrder == SortOrder.Ascending) {
                    return String.Compare(x.SubItems[_col].Text, y.SubItems[_col].Text,
                        StringComparison.CurrentCulture
                    );
                }
                else {
                    return String.Compare(y.SubItems[_col].Text, x.SubItems[_col].Text,
                        StringComparison.CurrentCulture
                    );
                }
            }
        }

        private void lviewDetails_DoubleClick(object sender, EventArgs e)
        {
            if (buttonChangeAttr.Enabled)
            {
                ChangeSelectedAttribute();
            }            
        }
    }    
}
