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
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Threading;
using WpfAdBook.DAL;
using WpfAdBook.ViewModels;

namespace WpfAdBook.Services
{
    public class DataService : IDataService
    {
        private System.Timers.Timer _timer = new System.Timers.Timer();
        private List<AdPersonVM> _dataPortion = new List<AdPersonVM>();
        private SynchronizationContext _callerContext;
        private ISettingsService _settingsService;
        private IRepository _repo;
        private int _numFound;

        public event EventHandler<DataPortionAvaiableEventArgs> DataPortionAvailable;
        public event EventHandler SearchStarted;
        public event EventHandler SearchStopped;

        public DataService(ISettingsService settingsService, IRepository repo)
        {
            _settingsService = settingsService;
            _repo = repo;
            _repo.SearchStarted += _repo_SearchStarted;
            _repo.SearchStopped += _repo_SearchStopped;
            _repo.ItemFound += _repo_ItemFound;

            _timer.Interval = 200;
            _timer.Elapsed += _timer_Elapsed;
        }

        public bool IsDataAccessible()
        {
            return _repo.IsAvailable(_settingsService.ConnectionParams);
        }

        public bool CheckConnectionParams(ConnectionParams connectionParams)
        {
            if (connectionParams == null) {
                throw new ArgumentNullException(nameof(connectionParams));
            }
            return _repo.IsAvailable(connectionParams);
        }

        private void HandleNewPortion(object state)
        {
            lock (_dataPortion) {
                if (_dataPortion.Count == 0) {
                    return;
                }
                EventHandler<DataPortionAvaiableEventArgs> dataPortionAvailable = DataPortionAvailable;
                if (dataPortionAvailable != null) {
                    var args = new DataPortionAvaiableEventArgs();
                    args.AdpList = new List<AdPersonVM>();
                    args.AdpList.AddRange(_dataPortion);
                    dataPortionAvailable(this, args);
                    _numFound += _dataPortion.Count;
                    _dataPortion.Clear();
                }
            }
        }

        private void _timer_Elapsed(object sender, System.Timers.ElapsedEventArgs e)
        {
            if (null != _callerContext) {
                _callerContext.Post(HandleNewPortion, null);
            }
            else {
                HandleNewPortion(null);
            }
        }

        private void _repo_ItemFound(object sender, ItemFoundEventArgs e)
        {
            lock (_dataPortion) {
                _dataPortion.Add(new AdPersonVM(e.Adp));
            }
        }

        private void HandleSearchStopped(object state)
        {
            _timer.Stop();
            HandleNewPortion(null);
            SearchStopped?.Invoke(this, EventArgs.Empty);
#if DEBUG
            Debug.WriteLine("DataService: num object: {0}", _numFound);
#endif
        }

        private void _repo_SearchStopped(object sender, EventArgs e)
        {
            if (_callerContext != null) {
                _callerContext.Post(HandleSearchStopped, null);
            }
            else {
                HandleSearchStopped(null);
            }
        }

        private void HandleSearchStarted(object state)
        {
            _timer.Start();
            SearchStarted?.Invoke(this, EventArgs.Empty);
            _numFound = 0;
        }

        private void _repo_SearchStarted(object sender, EventArgs e)
        {
            if (null != _callerContext) {
                _callerContext.Post(HandleSearchStarted, null);
            }
            else {
                HandleSearchStarted(null);
            }
        }

        public void CancelSearch()
        {
            _repo.CancelSearch();
        }

        public void EnsureSearchIsStopped()
        {
            _repo.EnsureSearchIsStopped();
        }

        public bool IsSearchActive()
        {
            return _repo.IsSearchActive();
        }

        public Task StartSearchAsync(LdapRequestBuilder ldapRequest)
        {
            if (ldapRequest == null) {
                throw new ArgumentNullException(nameof(ldapRequest));
            }
             _callerContext = SynchronizationContext.Current;
            return _repo.StartSearchAsync(_settingsService.ConnectionParams, ldapRequest);
        }

        public IEnumerable<AttrId> WriteAdPerson(AdPersonVM adp)
        {
            if (adp == null) {
                throw new ArgumentNullException(nameof(adp));
            }
            return _repo.WriteAdPerson(_settingsService.ConnectionParams, adp.GetUnderlyingType());
        }

        public AdPersonVM ReadAdPerson(string dn)
        {
            if (string.IsNullOrWhiteSpace(dn)) {
                throw new ArgumentException(nameof(dn));
            }
            return new AdPersonVM(_repo.ReadAdPerson(_settingsService.ConnectionParams, dn));
        }

        #region IDisposable Support
        private bool disposedValue = false; // To detect redundant calls

        protected virtual void Dispose(bool disposing)
        {
            if (!disposedValue) {
                if (disposing) {
                    EnsureSearchIsStopped();

                    _repo.SearchStarted -= _repo_SearchStarted;
                    _repo.SearchStopped -= _repo_SearchStopped;
                    _repo.ItemFound -= _repo_ItemFound;

                    _repo = null;
                    _timer.Stop();
                    _timer.Dispose();
                    _timer = null;
                }
                disposedValue = true;
            }
        }

        // This code added to correctly implement the disposable pattern.
        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        #endregion
    }
}
