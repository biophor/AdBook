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
using Microsoft.VisualStudio.TestTools.UnitTesting;
using WpfAdBook.DAL;
using adbookcli;
using System.Threading.Tasks;
using System.Collections.Generic;
using System.Diagnostics;
using System.Threading;

namespace WpfAdBookTests
{
    [TestClass]
    public class RepositoryTest
    {        
        [TestMethod]
        public void RepositoryTest_Search_CorrectArgs_RepoAvailable()
        {
            using (var connectionParams = new ConnectionParams())
            using (var ldapRequest = new LdapRequestBuilder())
            using (IRepository repository = new MockRepository()) {
                bool itemsFound = false;
                var retrievedData = new List<AdPerson>();
                repository.ItemFound += (object sender, ItemFoundEventArgs e) => {
                    retrievedData.Add(e.Adp);
                    itemsFound = true;
                };
                bool searchStarted = false;
                bool searchStopped = false;
                repository.SearchStarted += (object sender, EventArgs e) => { searchStarted = true; };
                repository.SearchStopped += (object sender, EventArgs e) => { searchStopped = true; };

                using (Task task = repository.StartSearchAsync(connectionParams, ldapRequest)) {
                    task.Wait();
                    Assert.IsTrue(searchStarted);
                    Assert.IsTrue(searchStopped);
                    Assert.IsTrue(itemsFound);
                    retrievedData.ForEach((AdPerson adp) => { adp.Dispose(); });
                }
            }
            
        }
        
        [TestMethod]
        public void RepositoryTest_Search_CorrectArgs_RepoUnavailable()
        {
            bool requiredExceptionWasThrown = false;
            using (var connectionParams = new ConnectionParams())
            using (var ldapRequest = new LdapRequestBuilder())
            using (IRepository repository = new MockRepository(true, false))
            using (Task task = repository.StartSearchAsync(connectionParams, ldapRequest)) {
                try {
                    task.Wait();
                }
                catch(AggregateException e) {
                    if (e.InnerException is AdSearchTaskError || e.InnerException is RepositoryInaccessibleError) {
                        requiredExceptionWasThrown = true;
                    }
                    else {
                        throw;
                    }
                }                
            }
            Assert.IsTrue(requiredExceptionWasThrown);
        }

        [TestMethod]
        public void RepositoryTest_Search_IncorrectArgs_RepoAvailable()
        {
            bool requiredExceptionWasThrown = false;
            using (var connectionParams = new ConnectionParams())
            using (var ldapRequest = new LdapRequestBuilder())
            using (IRepository repository = new MockRepository(false, true)) {
                try {
                    using (Task task = repository.StartSearchAsync(connectionParams, ldapRequest)) {
                        task.Wait();
                    }
                }
                catch (ArgumentException) {
                    requiredExceptionWasThrown = true;
                }            
            }
            Assert.IsTrue(requiredExceptionWasThrown);
        }
        
        [TestMethod]
        public void RepositoryTest_CancelSearch()
        {
            using (var connectionParams = new ConnectionParams())
            using (var ldapRequest = new LdapRequestBuilder())
            using (IRepository repository = new MockRepository()) {
                bool itemsFound = false;
                var retrievedData = new List<AdPerson>();
                repository.ItemFound += (object sender, ItemFoundEventArgs e) => {
                    retrievedData.Add(e.Adp);
                    itemsFound = true;
                };
                bool searchStarted = false;
                bool searchStopped = false;
                repository.SearchStarted += (object sender, EventArgs e) => { searchStarted = true; };
                repository.SearchStopped += (object sender, EventArgs e) => { searchStopped = true; };

                using (Task task = repository.StartSearchAsync(connectionParams, ldapRequest)) {
                    Thread.Sleep(1000);
                    repository.EnsureSearchIsStopped();                    
                    Assert.IsTrue(searchStarted);
                    Assert.IsTrue(searchStopped);
                    Assert.IsTrue(itemsFound);
                    retrievedData.ForEach((AdPerson adp) => { adp.Dispose(); });
                }
            }

        }

        [TestInitialize()]
        public void MyTestInitialize()
        {

        }

        [TestCleanup()]
        public void MyTestCleanup()
        {

        }
    }
}
