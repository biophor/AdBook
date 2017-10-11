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
using System.Collections.Generic;
using System.Threading.Tasks;
using adbookcli;
using System.Collections.ObjectModel;
using System.Threading;
using System.Windows.Input;
using System.ComponentModel;
using WpfAdBook.Services;
using WpfAdBook.DAL;
using System.IO;
using System.Runtime.Serialization.Formatters.Binary;
using System.Runtime.CompilerServices;

namespace WpfAdBook.ViewModels
{
    public class MainWindowVM: IDisposable, INotifyPropertyChanged
    {
        private readonly IDataService _dataService;
        private Task _searchTask;
        private LdapRequestBuilder _ldapRequst;
        private readonly IDialogService _dialogService;
        private readonly ISettingsService _settingsService;
        private readonly SynchronizationContext _callerContext;

        public ICommand StartSearchTaskCommand { get; }
        public ICommand AddFilterCommand { get; }
        public ICommand RemoveFilterCommand { get; }

        public ICommand CancelSearchCommand { get; }
        public ICommand DisplaySettingsWindowCommand { get; }
        public ICommand EditCommand { get; }

        public ICommand CloseMainWindowCommand { get; }
        public ICommand WindowClosedNotification { get; }
        public ICommand WindowClosingNotification { get; }
        public ICommand WindowLoadedNotification { get; }

        public ICommand DisplayAboutBoxCommand { get; }

        public ObservableCollection<AdPersonVM> PersonCollection { get; } = new ObservableCollection<AdPersonVM>();
        public PredefinedMatchingRulesVM PredefinedMatchingRules { get; }= new PredefinedMatchingRulesVM();
        public PredefinedFilterListVM PredefinedFilterList { get; } = new PredefinedFilterListVM();

        private FilterVM _selectedFilter;
        public FilterVM SelectedFilter {
            get => _selectedFilter;
            set {
                if (_selectedFilter?.Equals(value) == true) {
                    return;
                }
                _selectedFilter = value;
                RaisePropertyChanged(nameof(SelectedFilter));
            }
        }

        private bool _allFiltersShouldBeMet = false;
        public bool AllFilterRulesShouldBeMet {
            get => _allFiltersShouldBeMet;
            set {
                if (_allFiltersShouldBeMet != value) {
                    _allFiltersShouldBeMet = value;
                    RaisePropertyChanged(nameof(AllFilterRulesShouldBeMet));
                }
            }
        }

        private FilterMatchingRuleVM _selectedMatchingRule;
        public FilterMatchingRuleVM SelectedMatchingRule {
            get => _selectedMatchingRule;
            set {
                if (_selectedMatchingRule?.Equals(value) == true) {
                    return;
                }
                _selectedMatchingRule = value;
                RaisePropertyChanged(nameof(SelectedMatchingRule));
            }
        }

        public string CurrentFilterValue { get; set; }

        private AdPersonVM _selectedPerson;
        public AdPersonVM SelectedPerson {
            get => _selectedPerson;
            set {
                if (_selectedPerson != value) {
                    _selectedPerson = value;
                    RaisePropertyChanged(nameof(SelectedPerson));
                }
            }
        }

        private FilterRuleVM _selectedFilterRule;
        public FilterRuleVM SelectedFilterRule {
            get => _selectedFilterRule;
            set {
                if (_selectedFilterRule != value) {
                    _selectedFilterRule = value;
                    RaisePropertyChanged(nameof(SelectedFilterRule));
                }

            }
        }

        public ObservableCollection<FilterRuleVM> FilterRules { get; internal set; } = new ObservableCollection<FilterRuleVM>();

        public AdAttributesVM AdAttributes { get; } = new AdAttributesVM();

        public MainWindowVM(IDataService dataService, IDialogService dialogService, ISettingsService settingsService)
        {
            StartSearchTaskCommand = new CustomCommand(ExecuteStartSearchTaskCommandAsync, CanExecuteStartSearchTaskCommand);
            CancelSearchCommand = new CustomCommand(ExecuteCancelSearchCommand, CanExecuteCancelSearchCommand);
            DisplaySettingsWindowCommand = new CustomCommand(ExecuteDisplaySettingsWindowCommand, CanExecuteDisplaySettingsWindowCommand);
            EditCommand = new CustomCommand(ExecuteEditCommand, CanExecuteEditCommand);
            AddFilterCommand = new CustomCommand(ExecuteAddFilterCommand, CanExecuteAddFilterCommand);
            RemoveFilterCommand = new CustomCommand(ExecuteRemoveFilterCommand, CanExecuteRemoveFilterCommand);
            CloseMainWindowCommand = new CustomCommand(ExecuteCloseMainWindowCommand, CanExecuteCloseMainWindowCommand);
            WindowClosedNotification = new CustomCommand(OnWindowClosed);
            WindowClosingNotification = new CustomCommand(OnWindowClosing);
            WindowLoadedNotification = new CustomCommand(OnWindowLoaded);
            DisplayAboutBoxCommand = new CustomCommand(ExecuteDisplayAboutBoxCommand);

            _callerContext = SynchronizationContext.Current;
            _dialogService = dialogService;
            _dataService = dataService;
            _settingsService = settingsService;

            _dataService.DataPortionAvailable += _dataService_DataPortionAvailable;
            LoadFilterRules();
            PersonCollection = _settingsService.SavedSearchResult;
        }

        private void OnWindowLoaded(object obj)
        {
            _dialogService.RestoreMainWindowState();
        }

        private void ExecuteDisplayAboutBoxCommand(object obj)
        {
            _dialogService.DisplayAboutBox();
        }

        public void WaitUntilSearchIsOver()
        {
            try {
                _searchTask?.Wait();
            }
            catch (System.AggregateException e) {
                bool expectedError = e.InnerException is OperationCanceledException || e.InnerException is AdSearchTaskError ||
                    e.InnerException is RepositoryInaccessibleError || e.InnerException is DataModificationError;
                if (!expectedError) {
                    throw;
                }
            }
        }

        private void OnWindowClosing(object obj)
        {
#if DEBUG
            bool confirmed = _dialogService.RequestConfirmationOnAppExit();
            if (!confirmed) {
                var eventArgs = (CancelEventArgs)obj;
                eventArgs.Cancel = true;
                return;
            }
#endif
            using (var wc = new WaitCursor()) {
                _dialogService.SaveMainWindowState();
                _dataService.EnsureSearchIsStopped();
            }
        }

        private void OnWindowClosed(object obj)
        {

        }

        private void ExecuteCloseMainWindowCommand(object obj)
        {
            _dialogService.CloseMainWindow();
        }

        private bool CanExecuteCloseMainWindowCommand(object arg)
        {
            if (null == _searchTask) {
                return true;
            }
            return _searchTask.IsCompleted;
        }

        private bool CanExecuteRemoveFilterCommand(object arg)
        {
            return (_searchTask == null && SelectedFilterRule != null);
        }

        private void ExecuteRemoveFilterCommand(object obj)
        {
            FilterRules.Remove(SelectedFilterRule);
        }

        private void ExecuteAddFilterCommand(object obj)
        {
            var filterRule = new FilterRuleVM(SelectedFilter, SelectedMatchingRule, CurrentFilterValue);
            if (!FilterRules.Contains(filterRule)) {
                FilterRules.Add(filterRule);
            }
        }

        private bool CanExecuteAddFilterCommand(object arg)
        {
            if (null == CurrentFilterValue) {
                return false;
            }
            return (CurrentFilterValue.Trim().Length > 0) && (_searchTask == null);
        }

        private bool CanExecuteStartSearchTaskCommand(object arg)
        {
            return _searchTask == null;
        }

        private LdapRequestBuilder ConstructLdapRequest()
        {
            IVisitor visitor = new Visitor();
            var ldapReq = new LdapRequestBuilder();
            foreach(FilterRuleVM filterRule in FilterRules) {
                filterRule.Filter.Accept(visitor, ldapReq, filterRule.MatchingRule, filterRule.Value);
            }
            if (FilterRules.Count != 0) {
                if (AllFilterRulesShouldBeMet) {
                    ldapReq.AddAND();
                }
                else {
                    ldapReq.AddOR();
                }
            }
            ldapReq.AddRule("objectCategory", LdapRequestBuilder.MatchingRule.ExactMatch, "person");
            if (FilterRules.Count != 0) {
                ldapReq.AddAND();
            }
            return ldapReq;
        }

        private async void ExecuteStartSearchTaskCommandAsync(object obj)
        {
            try {
                ClearCollection();
                _ldapRequst?.Dispose();
                _ldapRequst = null;
                _ldapRequst = ConstructLdapRequest();
                string s = _ldapRequst.AsString();
                using (_searchTask = _dataService.StartSearchAsync(_ldapRequst)) {
                    await _searchTask;
                }
            }
            catch(AdSearchTaskError searchError) {
                _dialogService.DisplayErrorMessage(searchError.Message);
            }
            catch(RepositoryInaccessibleError accessError) {
                _dialogService.DisplayErrorMessage(accessError.Message);
            }
            catch (OperationCanceledException) {
                // not an error. Search has been canceled.
            }
            finally {
                _searchTask = null;
                CommandManager.InvalidateRequerySuggested();
            }
        }

        private bool CanExecuteEditCommand(object arg)
        {
            return SelectedPerson != null;
        }

        private void ExecuteEditCommand(object obj)
        {
            ViewModelMediator.Default.Send<AdPersonVM>(SelectedPerson);
            _dialogService.DisplayEditPersonWindow();
        }

        private void SaveLoadedPersonsInformation()
        {
            _settingsService.SavedSearchResult = PersonCollection;
        }

        private void _dataService_DataPortionAvailable(object sender, DataPortionAvaiableEventArgs e)
        {
            foreach (var v in e.AdpList) {
                PersonCollection.Add(v);
            }
        }

        private bool CanExecuteDisplaySettingsWindowCommand(object arg)
        {
            return _searchTask == null;
        }

        private void ExecuteDisplaySettingsWindowCommand(object obj)
        {
            _dialogService.DisplaySettingsWindow();
        }

        private bool CanExecuteCancelSearchCommand(object arg)
        {
            return _dataService.IsSearchActive();
        }

        private void ExecuteCancelSearchCommand(object obj)
        {
            _dataService.CancelSearch();
        }

        private void ClearCollection()
        {
            var tmp = new List<AdPersonVM>();
            foreach (AdPersonVM v in PersonCollection) {
                tmp.Add(v);
            }
            PersonCollection.Clear();
            foreach (AdPersonVM v in tmp) {
                v.Dispose();
            }
        }

        public event PropertyChangedEventHandler PropertyChanged;

        private void RaisePropertyChanged([CallerMemberName]string propertyName="")
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }

        private void LoadFilterRules()
        {
            if (string.IsNullOrWhiteSpace(_settingsService.FilteringParams.SerializedFilterRules)) {
                return;
            }
            byte[] data = Convert.FromBase64String(_settingsService.FilteringParams.SerializedFilterRules);

            using(MemoryStream ms = new MemoryStream(data)) {
                var bf = new BinaryFormatter();
                FilterRules = (ObservableCollection<FilterRuleVM>)bf.Deserialize(ms);
            }
            AllFilterRulesShouldBeMet = _settingsService.FilteringParams.StrictFiltering;
        }

        private void SaveFilterRules()
        {
            var bf = new BinaryFormatter();
            using (var ms = new MemoryStream()) {
                bf.Serialize(ms, FilterRules);
                _settingsService.FilteringParams.SerializedFilterRules = Convert.ToBase64String(ms.GetBuffer());
                _settingsService.FilteringParams.StrictFiltering = AllFilterRulesShouldBeMet;
            }
        }

#region IDisposable Support
        private bool disposedValue = false; // To detect redundant calls

        protected virtual void Dispose(bool disposing)
        {
            if (!disposedValue) {
                if (disposing) {
                    SaveFilterRules();

                    _dataService.DataPortionAvailable -= _dataService_DataPortionAvailable;

                    _settingsService.Save();

                    foreach (var v in PersonCollection) {
                        v.Dispose();
                    }
                    _ldapRequst?.Dispose();
                }
                disposedValue = true;
            }
        }
        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }
#endregion
    }
}

