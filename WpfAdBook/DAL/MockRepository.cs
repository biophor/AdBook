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
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Timers;
using WpfAdBook.ViewModels;

namespace WpfAdBook.DAL
{
    // This class is used in several unit tests to improve readability
    public sealed class MockRepository : IRepository
    {
        private const int searchTimeInSeconds = 4;
        private bool _simulateInaccessibleRepo = false;
        private bool _simulateIncorrectSearchArgs = false;
        private List<AdPerson> _predefinedPeople;
        private CancellationTokenSource _cts;
        private Task _searchTask;
        public event EventHandler<ItemFoundEventArgs> ItemFound;
        public event EventHandler SearchStarted;
        public event EventHandler SearchStopped;

        public bool UseDelay { get; set; } = true;

        public int NumPersonInRepo {
            get {
                return _predefinedPeople.Count;
            }
        }

        public MockRepository(bool simulateInaccessibleRepo = false, bool simulateIncorrectSearchArgs = false)
        {
            _simulateInaccessibleRepo = simulateInaccessibleRepo;
            _simulateIncorrectSearchArgs = simulateIncorrectSearchArgs;
            //_predefinedPeople
            _predefinedPeople = new List<AdPerson>();
            AdPerson person = new AdPerson();
            person.set_StringAttr("cn", "John");
            person.set_StringAttr("mail", "john@mail.ru");
            person.set_StringAttr("company", "Microsoft");
            person.set_StringAttr("title", "programmer");
            person.set_StringAttr("department", "R & D");
            person.set_StringAttr("st", "State Name");
            person.set_StringAttr("employeeID", "1");
            _predefinedPeople.Add(person);

            person = new AdPerson();
            person.set_StringAttr("cn", "Kyle");
            person.set_StringAttr("mail", "kyle@mail.ru");
            person.set_StringAttr("company", "IBM");
            person.set_StringAttr("title", "programmer");
            person.set_StringAttr("department", "R & D");
            person.set_StringAttr("employeeID", "8");
            _predefinedPeople.Add(person);

            person = new AdPerson();
            person.set_StringAttr("cn", "Mike");
            person.set_StringAttr("mail", "mike@mail.ru");
            person.set_StringAttr("company", "Microsoft");
            person.set_StringAttr("title", "lead programmer");
            person.set_StringAttr("department", "R & D");
            person.set_StringAttr("employeeID", "2");
            _predefinedPeople.Add(person);

            person = new AdPerson();
            person.set_StringAttr("cn", "Jack");
            person.set_StringAttr("mail", "Jack@mail.ru");
            person.set_StringAttr("company", "Apple");
            person.set_StringAttr("title", "CTO");
            person.set_StringAttr("department", "Top managment");
            person.set_StringAttr("employeeID", "3");
            _predefinedPeople.Add(person);

            person = new AdPerson();
            person.set_StringAttr("cn", "Sara");
            person.set_StringAttr("mail", "Sara@mail.ru");
            person.set_StringAttr("company", "Apple");
            person.set_StringAttr("title", "lead designer");
            person.set_StringAttr("department", "R & D");
            person.set_StringAttr("employeeID", "4");
            _predefinedPeople.Add(person);

            person = new AdPerson();
            person.set_StringAttr("cn", "Cole");
            person.set_StringAttr("mail", "cole@mail.ru");
            person.set_StringAttr("company", "IBM");
            person.set_StringAttr("title", "manager");
            person.set_StringAttr("department", "Managment");

            person.set_StringAttr("employeeID", "5");
            _predefinedPeople.Add(person);
        }

        public bool IsAvailable(ConnectionParams connectionParams)
        {
            return !_simulateInaccessibleRepo;
        }

        public void CancelSearch()
        {
            _cts?.Cancel();
        }

        public void EnsureSearchIsStopped()
        {
            _cts?.Cancel();
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
            _searchTask?.Dispose();
            _cts?.Dispose();
            _cts = null;
        }

        public bool IsSearchActive()
        {
            return (_searchTask != null) && (!_searchTask.IsCompleted);
        }

        #region IDisposable Support
        private bool disposedValue = false; // To detect redundant calls

        public void Dispose()
        {
            if (!disposedValue) {
                EnsureSearchIsStopped();
                _predefinedPeople.ForEach((adp) => { adp.Dispose(); });
                _predefinedPeople.Clear();
                _cts.Dispose();
                _searchTask.Dispose();
                disposedValue = true;
            }
        }
        #endregion

        public IEnumerable<AttrId> WriteAdPerson(ConnectionParams connectionParams, AdPerson adp)
        {
            throw new NotImplementedException();
        }

        public AdPerson ReadAdPerson(ConnectionParams connectionParams, string dn)
        {
            throw new NotImplementedException();
        }

        public Task StartSearchAsync(ConnectionParams connectionParams, LdapRequestBuilder ldapRequest)
        {
            if (_simulateIncorrectSearchArgs) {
                throw new ArgumentException();
            }
            EnsureSearchIsStopped();
            _cts = new CancellationTokenSource();
            _searchTask = new Task(
                (object state) => {
                    CancellationToken ct = (CancellationToken)state;
                    try {
                        if (_simulateInaccessibleRepo) {
                            throw new RepositoryInaccessibleError(Properties.Resources.RepositoryInaccessibleError);
                        }
                        SearchStarted?.Invoke(this, EventArgs.Empty);
                        int sleepPerPerson = searchTimeInSeconds * 1000 / _predefinedPeople.Count;
                        foreach (AdPerson v in _predefinedPeople) {
                            ItemFound?.Invoke(this, new ItemFoundEventArgs() { Adp = (AdPerson)v.Clone() });
                            ct.ThrowIfCancellationRequested();
                            if (UseDelay) {
                                Thread.Sleep(sleepPerPerson);
                            }
                        }
                    }
                    finally {
                        SearchStopped?.Invoke(this, EventArgs.Empty);
                    }
                },
                _cts.Token,
                _cts.Token
            );
            _searchTask.Start();
            return _searchTask;
        }


    }
}
