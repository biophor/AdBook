/*
Copyright (C) 2015-2020 Andrei Goncharov.

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
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using adbookcli;
using NUnit;
using Moq;
using NUnit.Framework;
using WpfAdBook.DAL;
using WpfAdBook.Services;
using System.Threading;
using WpfAdBook.ViewModels;

// how to setup the test environment: '\AdBook\AdBookBL_IntegrationTests\How to setup the test environment on Windows 10.txt'

namespace WpfAdBook_IntegrationTests
{
    [TestFixture]
    public class DataService_Tests
    {
        [Test]
        public void CanFindExistingPersonIfDataSourceIsAccessible()
        {
            // Arrange
            Mock<ISettingsService> settingsService = new Mock<ISettingsService>();
            using (var connectionParams = new ConnectionParams())
            using (var repo = new AdRepository())
            using (var ldapRequestBuilder = new LdapRequestBuilder())
            using (var dataService = new DataService(settingsService.Object, repo)) {

                connectionParams.UseCurrentUserCredentials = true;
                connectionParams.ConnectDomainYouLoggedIn = false;
                connectionParams.LdapPath = "LDAP://localhost:55555/CN=users,DC=testdata,DC=adbook,DC=local";

                settingsService.Setup(ss => ss.ConnectionParams).Returns(connectionParams);

                ldapRequestBuilder.AddRule("cn", LdapRequestBuilder.MatchingRule.ExactMatch, "Kristie Etherton");
                ldapRequestBuilder.AddRule("objectCategory", LdapRequestBuilder.MatchingRule.ExactMatch, "person");
                ldapRequestBuilder.AddAND();

                int searchStarted = 0;
                int searchStopped = 0;
                int itemFound = 0;
                dataService.SearchStarted += delegate { ++searchStarted; };
                dataService.SearchStopped += delegate { ++searchStopped; };

                dataService.DataPortionAvailable +=
                    (object sender, DataPortionAvaiableEventArgs args) =>
                        {
                            args.AdpList.ForEach( adp => adp.Dispose() );
                            itemFound += args.AdpList.Count;
                        };
                // Act
                var searchTask = dataService.StartSearchAsync(ldapRequestBuilder);
                searchTask.Wait();

                // Assert
                Assert.IsTrue(searchStarted == 1);
                Assert.IsTrue(searchStopped == 1);
                Assert.IsTrue(itemFound == 1);
            }
        }

        [Test]
        public void CantFindNonExistingPersonIfDataSourceIsAccessible()
        {
            // Arrange
            Mock<ISettingsService> settingsService = new Mock<ISettingsService>();
            using (var connectionParams = new ConnectionParams())
            using (var repo = new AdRepository())
            using (var ldapRequestBuilder = new LdapRequestBuilder())
            using (var dataService = new DataService(settingsService.Object, repo)) {

                connectionParams.UseCurrentUserCredentials = true;
                connectionParams.ConnectDomainYouLoggedIn = false;
                connectionParams.LdapPath = "LDAP://localhost:55555/CN=users,DC=testdata,DC=adbook,DC=local";

                settingsService.Setup(ss => ss.ConnectionParams).Returns(connectionParams);

                ldapRequestBuilder.AddRule("cn", LdapRequestBuilder.MatchingRule.ExactMatch, "Invalid Name");
                ldapRequestBuilder.AddRule("objectCategory", LdapRequestBuilder.MatchingRule.ExactMatch, "person");
                ldapRequestBuilder.AddAND();

                int searchStarted = 0;
                int searchStopped = 0;
                int itemFound = 0;
                dataService.SearchStarted += delegate { ++searchStarted; };
                dataService.SearchStopped += delegate { ++searchStopped; };

                dataService.DataPortionAvailable +=
                    (object sender, DataPortionAvaiableEventArgs args) =>
                    {
                        args.AdpList.ForEach(adp => adp.Dispose());
                        itemFound += args.AdpList.Count;
                    };

                // Act
                var searchTask = dataService.StartSearchAsync(ldapRequestBuilder);
                searchTask.Wait();

                // Assert
                Assert.IsTrue(searchStarted == 1);
                Assert.IsTrue(searchStopped == 1);
                Assert.IsTrue(itemFound == 0);
            }
        }

        [Test]
        public void ThrowExceptionIfDataSourceIsInaccessibleDuringSearch()
        {
            // Arrange
            Mock<ISettingsService> settingsService = new Mock<ISettingsService>();
            using (var connectionParams = new ConnectionParams())
            using (var repo = new AdRepository())
            using (var ldapRequestBuilder = new LdapRequestBuilder())
            using (var dataService = new DataService(settingsService.Object, repo)) {
                connectionParams.UseCurrentUserCredentials = true;
                connectionParams.ConnectDomainYouLoggedIn = false;
                connectionParams.LdapPath = "LDAP://localhost:55555/CN=users,DC=testdata,DC=INVALID,DC=INVALID";
                ldapRequestBuilder.AddRule("objectCategory", LdapRequestBuilder.MatchingRule.ExactMatch, "person");
                ldapRequestBuilder.AddRule("cn", LdapRequestBuilder.MatchingRule.ExactMatch, "Kristie Etherton");
                ldapRequestBuilder.AddAND();

                settingsService.Setup(ss => ss.ConnectionParams).Returns(connectionParams);

                // Act & Assert
                Assert.ThrowsAsync(typeof(RepositoryInaccessibleError), () => dataService.StartSearchAsync(ldapRequestBuilder));
            }
        }

        [Test]
        public async Task CanCancelTheSearch()
        {
            // Arrange
            Mock<ISettingsService> settingsService = new Mock<ISettingsService>();
            using (var connectionParams = new ConnectionParams())
            using (var repo = new AdRepository())
            using (var ldapRequestBuilder = new LdapRequestBuilder())
            using (var dataService = new DataService(settingsService.Object, repo)) {
                connectionParams.UseCurrentUserCredentials = true;
                connectionParams.ConnectDomainYouLoggedIn = false;
                connectionParams.LdapPath = "LDAP://localhost:55555/CN=users,DC=testdata,DC=adbook,DC=local";
                ldapRequestBuilder.AddRule("objectCategory", LdapRequestBuilder.MatchingRule.ExactMatch, "person");

                settingsService.Setup(ss => ss.ConnectionParams).Returns(connectionParams);

                dataService.DataPortionAvailable +=
                    (object sender, DataPortionAvaiableEventArgs args) => {
                        args.AdpList.ForEach(adp => adp.Dispose());
                        dataService.CancelSearch();
                    };
                // Act
                var searchTask = repo.StartSearchAsync(connectionParams, ldapRequestBuilder);
                try {
                    await searchTask;
                }
                catch (OperationCanceledException) {

                }
                Assert.IsTrue(searchTask.IsCanceled);
            }
        }

        [Test]
        public void CanChangeTheAttributesOfExistingPerson()
        {
            // Arrange
            Mock<ISettingsService> settingsService = new Mock<ISettingsService>();
            using (var connectionParams = new ConnectionParams())
            using (var repo = new AdRepository())
            using (var ldapRequestBuilder = new LdapRequestBuilder())
            using (var dataService = new DataService(settingsService.Object, repo)) {

                connectionParams.UseCurrentUserCredentials = true;
                connectionParams.ConnectDomainYouLoggedIn = false;
                connectionParams.LdapPath = "LDAP://localhost:55555/CN=users,DC=testdata,DC=adbook,DC=local";
                ldapRequestBuilder.AddRule("cn", LdapRequestBuilder.MatchingRule.ExactMatch, "Kristie Etherton");
                ldapRequestBuilder.AddRule("objectCategory", LdapRequestBuilder.MatchingRule.ExactMatch, "person");
                ldapRequestBuilder.AddAND();

                settingsService.Setup(ss => ss.ConnectionParams).Returns(connectionParams);

                // Act
                AdPersonVM adp = null;

                dataService.DataPortionAvailable +=
                    (object sender, DataPortionAvaiableEventArgs args) => {
                        adp = args.AdpList[0];
                    };

                var searchTask = dataService.StartSearchAsync(ldapRequestBuilder);
                searchTask.Wait();

                using (adp) {
                    var empIdAttr = AdAttributes.Get(AttrId.EmpId);
                    var randomId = DateTime.Now.Millisecond;
                    string attrValueToBeWritten = randomId.ToString();
                    adp.SetStringAttr(empIdAttr.Id, attrValueToBeWritten);
                    IEnumerable<AttrId> skippedAttributes = dataService.WriteAdPerson(adp);

                    // Assert
                    Assert.IsFalse(skippedAttributes.Contains(AttrId.EmpId));

                    using (var adp2 = dataService.ReadAdPerson(adp.Dn)) {
                        string attrValueRead = adp.GetStringAttr(empIdAttr.Id);
                        Assert.IsTrue(attrValueToBeWritten == attrValueRead);
                    }
                }
            }
        }

        [Test]
        public void SkipAttributesNotAllowedToChangeWhenWritingAdPerson()
        {
            // Arrange
            Mock<ISettingsService> settingsService = new Mock<ISettingsService>();
            using (var connectionParams = new ConnectionParams())
            using (var repo = new AdRepository())
            using (var ldapRequestBuilder = new LdapRequestBuilder())
            using (var dataService = new DataService(settingsService.Object, repo)) {

                connectionParams.UseCurrentUserCredentials = true;
                connectionParams.ConnectDomainYouLoggedIn = false;
                connectionParams.LdapPath = "LDAP://localhost:55555/CN=users,DC=testdata,DC=adbook,DC=local";
                ldapRequestBuilder.AddRule("cn", LdapRequestBuilder.MatchingRule.ExactMatch, "Kristie Etherton");
                ldapRequestBuilder.AddRule("objectCategory", LdapRequestBuilder.MatchingRule.ExactMatch, "person");
                ldapRequestBuilder.AddAND();

                settingsService.Setup(ss => ss.ConnectionParams).Returns(connectionParams);

                AdPersonVM adp = null;
                dataService.DataPortionAvailable +=
                    (object sender, DataPortionAvaiableEventArgs args) => {
                        adp = args.AdpList[0];
                    };

                // Act
                var searchTask = dataService.StartSearchAsync(ldapRequestBuilder);
                searchTask.Wait();

                using (adp) {
                    // disallow the modification of the EmpId attribute
                    adp.SetAttributeWritable(AttrId.EmpId, false);
                    var empIdAttr = AdAttributes.Get(AttrId.EmpId);
                    adp.SetStringAttr(empIdAttr.Id, "123456");
                    IEnumerable<AttrId> skippedAttrs = dataService.WriteAdPerson(adp);

                    // Assert
                    Assert.IsTrue(skippedAttrs.Contains(AttrId.EmpId));
                }
            }
        }

        [Test]
        public void CanDetermineWhetherSearchIsActive()
        {
            // Arrange
            Mock<ISettingsService> settingsService = new Mock<ISettingsService>();
            using (var connectionParams = new ConnectionParams())
            using (var repo = new AdRepository())
            using (var ldapRequestBuilder = new LdapRequestBuilder())
            using (var dataService = new DataService(settingsService.Object, repo)) {

                connectionParams.UseCurrentUserCredentials = true;
                connectionParams.ConnectDomainYouLoggedIn = false;
                connectionParams.LdapPath = "LDAP://localhost:55555/CN=users,DC=testdata,DC=adbook,DC=local";
                ldapRequestBuilder.AddRule("objectCategory", LdapRequestBuilder.MatchingRule.ExactMatch, "person");

                settingsService.Setup(ss => ss.ConnectionParams).Returns(connectionParams);

                bool isSearchActive = false;

                dataService.DataPortionAvailable +=
                    (object sender, DataPortionAvaiableEventArgs args) => {
                        args.AdpList.ForEach(adp => adp.Dispose());
                        isSearchActive = dataService.IsSearchActive();
                };

                // Act
                var searchTask = dataService.StartSearchAsync(ldapRequestBuilder);
                searchTask.Wait();

                // Assert
                Assert.IsTrue(isSearchActive);
            }
        }

        [Test]
        public void CanStopTheSearch()
        {
            // Arrange
            Mock<ISettingsService> settingsService = new Mock<ISettingsService>();
            using (var connectionParams = new ConnectionParams())
            using (var repo = new AdRepository())
            using (var ldapRequestBuilder = new LdapRequestBuilder())
            using (var dataService = new DataService(settingsService.Object, repo)) {

                connectionParams.UseCurrentUserCredentials = true;
                connectionParams.ConnectDomainYouLoggedIn = false;
                connectionParams.LdapPath = "LDAP://localhost:55555/CN=users,DC=testdata,DC=adbook,DC=local";
                ldapRequestBuilder.AddRule("objectCategory", LdapRequestBuilder.MatchingRule.ExactMatch, "person");

                settingsService.Setup(ss => ss.ConnectionParams).Returns(connectionParams);

                dataService.DataPortionAvailable +=
                    (object sender, DataPortionAvaiableEventArgs args) => {
                        args.AdpList.ForEach(adp => adp.Dispose());
                        Thread.Sleep(1000);
                    };

                // Act
                var searchTask = dataService.StartSearchAsync(ldapRequestBuilder);
                dataService.EnsureSearchIsStopped();

                // Assert
                Assert.IsTrue(searchTask.IsCanceled);
            }
        }

        [Test]
        public void NewPortionNotifierIsCalledInTheSameCallerContext()
        {
            // Arrange
            SynchronizationContext callerContext = SynchronizationContext.Current;

            Mock<ISettingsService> settingsService = new Mock<ISettingsService>();
            using (var connectionParams = new ConnectionParams())
            using (var repo = new AdRepository())
            using (var ldapRequestBuilder = new LdapRequestBuilder())
            using (var dataService = new DataService(settingsService.Object, repo)) {
                connectionParams.UseCurrentUserCredentials = true;
                connectionParams.ConnectDomainYouLoggedIn = false;
                connectionParams.LdapPath = "LDAP://localhost:55555/CN=users,DC=testdata,DC=adbook,DC=local";
                ldapRequestBuilder.AddRule("cn", LdapRequestBuilder.MatchingRule.ExactMatch, "Kristie Etherton");
                ldapRequestBuilder.AddRule("objectCategory", LdapRequestBuilder.MatchingRule.ExactMatch, "person");
                ldapRequestBuilder.AddAND();

                settingsService.Setup(ss => ss.ConnectionParams).Returns(connectionParams);
                bool syncContextIsTheSame = false;
                dataService.DataPortionAvailable +=
                    (object sender, DataPortionAvaiableEventArgs args) => {
                        args.AdpList.ForEach(adp => adp.Dispose());
                        syncContextIsTheSame = (callerContext == SynchronizationContext.Current);
                    };

                // Act
                var searchTask = dataService.StartSearchAsync(ldapRequestBuilder);
                searchTask.Wait();

                // Assert
                Assert.IsTrue(syncContextIsTheSame);
            }
        }
    }
}
