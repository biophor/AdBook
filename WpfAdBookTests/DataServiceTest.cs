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
using System.Text;
using System.Collections.Generic;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using adbookcli;
using WpfAdBook.DAL;
using System.Threading.Tasks;
using WpfAdBook.Services;
using WpfAdBook.ViewModels;

namespace WpfAdBookTests
{
    /// <summary>
    /// Summary description for DataServiceTest
    /// </summary>
    [TestClass]
    public class DataServiceTest
    {        
        public DataServiceTest()
        {
            //
            // TODO: Add constructor logic here
            //
        }        

        private TestContext testContextInstance;

        /// <summary>
        ///Gets or sets the test context which provides
        ///information about and functionality for the current test run.
        ///</summary>
        public TestContext TestContext {
            get {
                return testContextInstance;
            }
            set {
                testContextInstance = value;
            }
        }
        
        //
        // You can use the following additional attributes as you write your tests:
        //
        // Use ClassInitialize to run code before running the first test in the class
        // [ClassInitialize()]
        // public static void MyClassInitialize(TestContext testContext) { }
        //
        // Use ClassCleanup to run code after all tests in a class have run
        // [ClassCleanup()]
        // public static void MyClassCleanup() { }
        //
        // Use TestInitialize to run code before running each test 
        [TestInitialize()]
        public void MyTestInitialize()
        {
     
        }

        //Use TestCleanup to run code after each test has run
        [TestCleanup()]
        public void MyTestCleanup()
        {
     
        }

        
        
        [TestMethod]
        public void DataServiceTest_Search_CorrectArgs_DataServiceAvailable()
        {
            ISettingsService settingService = new MockSettingsService();
            
            using (var ldapRequest = new LdapRequestBuilder())
            using (IRepository repository = new MockRepository())
            using (IDataService dataService = new DataService(settingService, repository)) {
                bool itemsFound = false;
                var retrievedData = new List<AdPersonVM>();
                dataService.DataPortionAvailable += (object sender, DataPortionAvaiableEventArgs e) => {
                    retrievedData.AddRange(e.AdpList);
                    Assert.IsTrue(e.AdpList.Count != 0);
                    itemsFound = true;
                };
                bool searchStarted = false;
                bool searchStopped = false;
                dataService.SearchStarted += (object sender, EventArgs e) => { searchStarted = true; };
                dataService.SearchStopped += (object sender, EventArgs e) => { searchStopped = true; };

                using (Task task = dataService.StartSearchAsync(ldapRequest)) {
                    task.Wait();
                    Assert.IsTrue(searchStarted);
                    Assert.IsTrue(searchStopped);
                    Assert.IsTrue(itemsFound);
                    retrievedData.ForEach((AdPersonVM adp) => { adp.Dispose(); });
                }
            }
        }

        [TestMethod]
        public void DataServiceTest_CorrectArgs_DataServiceUnavailable()
        {
            ISettingsService settingService = new MockSettingsService();

            using (var ldapRequest = new LdapRequestBuilder())
            using (IRepository repository = new MockRepository(true, false))
            using (IDataService dataService = new DataService(settingService, repository)) {
                bool itemsFound = false;
                var retrievedData = new List<AdPersonVM>();
                dataService.DataPortionAvailable += (object sender, DataPortionAvaiableEventArgs e) => {
                    retrievedData.AddRange(e.AdpList);
                    Assert.IsTrue(e.AdpList.Count != 0);
                    itemsFound = true;
                };                                
                bool requiredExceptionWasThrown = false;
                using (Task task = dataService.StartSearchAsync(ldapRequest)) {
                    try {
                        task.Wait();
                    }
                    catch (AggregateException e) {
                        if (e.InnerException is AdSearchTaskError || e.InnerException is RepositoryInaccessibleError) {
                            requiredExceptionWasThrown = true;
                        }
                        else {
                            throw;
                        }
                    }                    
                    Assert.IsFalse(itemsFound);
                    Assert.IsTrue(retrievedData.Count == 0);
                    Assert.IsTrue(requiredExceptionWasThrown == true);
                }
            }
        }

        [TestMethod]
        public void DataServiceTest_CancelSearch()
        {
            ISettingsService settingService = new MockSettingsService();

            using (var ldapRequest = new LdapRequestBuilder())
            using (IRepository repository = new MockRepository(false, false))
            using (IDataService dataService = new DataService(settingService, repository)) {                
                var retrievedData = new List<AdPersonVM>();
                dataService.DataPortionAvailable += (object sender, DataPortionAvaiableEventArgs e) => {
                    retrievedData.AddRange(e.AdpList);                    
                };                

                using (Task task = dataService.StartSearchAsync(ldapRequest)) {
                    dataService.EnsureSearchIsStopped();                    
                    retrievedData.ForEach((AdPersonVM adp) => { adp.Dispose(); });
                }
            }
        }
    }
}
