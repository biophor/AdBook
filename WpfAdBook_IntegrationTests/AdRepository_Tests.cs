/*
Copyright (C) 2015-2020 Goncharov Andrei.

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
using NUnit;
using NUnit.Framework;
using WpfAdBook.DAL;
using adbookcli;
using WpfAdBook.Services;
using WpfAdBook.ViewModels;
using System.Threading;

// how to setup the test environment: '\AdBook\AdBookBL_IntegrationTests\How to setup the test environment on Windows 10.txt'

namespace WpfAdBook_IntegrationTests
{
    [TestFixture]
    public class AdRepository_Tests
    {
        [Test]
        public void CanFindExistingPersonIfDataSourceIsAccessible()
        {
            // Arrange
            using (var connectionParams = new ConnectionParams())
            using (var repo = new AdRepository())
            using (var ldapRequestBuilder = new LdapRequestBuilder())
            {
                connectionParams.UseCurrentUserCredentials = true;
                connectionParams.UseDomainYouAreLoggedIn = false;
                connectionParams.LdapPath = "LDAP://localhost:55555/CN=users,DC=testdata,DC=adbook,DC=local";

                ldapRequestBuilder.AddRule("cn", LdapRequestBuilder.MatchingRule.ExactMatch, "Kristie Etherton");
                ldapRequestBuilder.AddRule("objectCategory", LdapRequestBuilder.MatchingRule.ExactMatch, "person");
                ldapRequestBuilder.AddAND();

                int searchStarted = 0;
                int searchStopped = 0;
                int itemFound = 0;
                repo.SearchStarted += delegate { ++searchStarted; };
                repo.SearchStopped += delegate { ++searchStopped; };
                AdPerson adp = null;
                repo.ItemFound += (object sender, ItemFoundEventArgs args) => { adp = args.Adp; ++itemFound; };

                // Act
                var searchTask = repo.StartSearchAsync(connectionParams, ldapRequestBuilder);
                searchTask.Wait();
                adp.Dispose();

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
            using (var connectionParams = new ConnectionParams())
            using (var repo = new AdRepository())
            using (var ldapRequestBuilder = new LdapRequestBuilder())
            {
                connectionParams.UseCurrentUserCredentials = true;
                connectionParams.UseDomainYouAreLoggedIn = false;
                connectionParams.LdapPath = "LDAP://localhost:55555/CN=users,DC=testdata,DC=adbook,DC=local";
                ldapRequestBuilder.AddRule("cn", LdapRequestBuilder.MatchingRule.ExactMatch, "INVALID NAME");
                ldapRequestBuilder.AddRule("objectCategory", LdapRequestBuilder.MatchingRule.ExactMatch, "person");
                ldapRequestBuilder.AddAND();

                int searchStarted = 0;
                int searchStopped = 0;
                int itemFound = 0;
                repo.SearchStarted += delegate { ++searchStarted; };
                repo.SearchStopped += delegate { ++searchStopped; };
                repo.ItemFound += (object sender, ItemFoundEventArgs args) => { ++itemFound; };

                // Act
                var searchTask = repo.StartSearchAsync(connectionParams, ldapRequestBuilder);
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
            using (var connectionParams = new ConnectionParams())
            using (var repo = new AdRepository())
            using (var ldapRequestBuilder = new LdapRequestBuilder())
            {
                connectionParams.UseCurrentUserCredentials = true;
                connectionParams.UseDomainYouAreLoggedIn = false;
                connectionParams.LdapPath = "LDAP://localhost:55555/CN=users,DC=testdata,DC=INVALID,DC=INVALID";
                ldapRequestBuilder.AddRule("cn", LdapRequestBuilder.MatchingRule.ExactMatch, "Kristie Etherton");
                ldapRequestBuilder.AddRule("objectCategory", LdapRequestBuilder.MatchingRule.ExactMatch, "person");
                ldapRequestBuilder.AddAND();

                bool expectedExceptionCaught = false;
                // Act
                try
                {
                    var searchTask = repo.StartSearchAsync(connectionParams, ldapRequestBuilder);
                    searchTask.Wait();
                }
                catch (RepositoryInaccessibleError)
                {
                    expectedExceptionCaught = true;
                }

                // Assert
                Assert.IsTrue(expectedExceptionCaught);
            }
        }

        [Test]
        public void CanDetermineIfTheDataSourceIsAvailable()
        {
            // Arrange
            using (var connectionParams = new ConnectionParams())
            using (var repo = new AdRepository())
            {
                connectionParams.UseCurrentUserCredentials = true;
                connectionParams.UseDomainYouAreLoggedIn = false;
                connectionParams.LdapPath = "LDAP://localhost:55555/CN=users,DC=testdata,DC=adbook,DC=local";

                // Act
                bool isAvailable = repo.IsAvailable(connectionParams);

                // Assert
                Assert.IsTrue(isAvailable);
            }
        }

        [Test]
        public void CanDetermineIfTheDataSourceIsNotAvailable()
        {
            // Arrange
            using (var connectionParams = new ConnectionParams())
            using (var repo = new AdRepository())
            {
                connectionParams.UseCurrentUserCredentials = true;
                connectionParams.UseDomainYouAreLoggedIn = false;
                connectionParams.LdapPath = "LDAP://localhost:55555/CN=users,DC=testdata,DC=INVALID,DC=INVALID";

                // Act
                bool isAvailable = repo.IsAvailable(connectionParams);

                // Assert
                Assert.IsFalse(isAvailable);
            }
        }

        [Test]
        public void CanChangeTheAttributesOfExistingPerson()
        {
            // Arrange
            using (var connectionParams = new ConnectionParams())
            using (var repo = new AdRepository())
            using (var ldapRequestBuilder = new LdapRequestBuilder())
            {
                connectionParams.UseCurrentUserCredentials = true;
                connectionParams.UseDomainYouAreLoggedIn = false;
                connectionParams.LdapPath = "LDAP://localhost:55555/CN=users,DC=testdata,DC=adbook,DC=local";
                ldapRequestBuilder.AddRule("cn", LdapRequestBuilder.MatchingRule.ExactMatch, "Kristie Etherton");
                ldapRequestBuilder.AddRule("objectCategory", LdapRequestBuilder.MatchingRule.ExactMatch, "person");
                ldapRequestBuilder.AddAND();
                // Act
                AdPerson adp = null;

                repo.ItemFound += (object sender, ItemFoundEventArgs args) => { adp = args.Adp; };
                var searchTask = repo.StartSearchAsync(connectionParams, ldapRequestBuilder);
                searchTask.Wait();

                using (adp)
                {
                    var empIdAttr = AdAttributes.Get(AttrId.EmpId);
                    var randomId = DateTime.Now.Millisecond;
                    string attrValueToBeWritten = randomId.ToString();
                    adp.set_StringAttr(empIdAttr.LdapName, attrValueToBeWritten);
                    IEnumerable<AttrId> skippedAttributes = repo.WriteAdPerson(connectionParams, adp);
                    // Assert
                    Assert.IsFalse(skippedAttributes.Contains(AttrId.EmpId));

                    using (var adp2 = repo.ReadAdPerson(connectionParams, adp.Dn)) {
                        string attrValueRead = adp.get_StringAttr(empIdAttr.LdapName);
                        Assert.IsTrue(attrValueToBeWritten == attrValueRead);
                    }
                }
            }
        }

        [Test]
        public void ThrowExceptionWhenTryingToChangeDnAttribute()
        {
            // Arrange
            using (var connectionParams = new ConnectionParams())
            using (var repo = new AdRepository())
            using (var ldapRequestBuilder = new LdapRequestBuilder())
            {
                connectionParams.UseCurrentUserCredentials = true;
                connectionParams.UseDomainYouAreLoggedIn = false;
                connectionParams.LdapPath = "LDAP://localhost:55555/CN=users,DC=testdata,DC=adbook,DC=local";
                ldapRequestBuilder.AddRule("cn", LdapRequestBuilder.MatchingRule.ExactMatch, "Kristie Etherton");
                ldapRequestBuilder.AddRule("objectCategory", LdapRequestBuilder.MatchingRule.ExactMatch, "person");
                ldapRequestBuilder.AddAND();
                // Act
                AdPerson adp = null;
                int numItemsFound = 0;
                repo.ItemFound += (object sender, ItemFoundEventArgs args) => { adp = args.Adp; ++numItemsFound; };
                var searchTask = repo.StartSearchAsync(connectionParams, ldapRequestBuilder);
                searchTask.Wait();

                using (adp)
                {
                    var distinguishedNameAttr = AdAttributes.Get(adbookcli.AttrId.Dn);
                    adp.set_StringAttr(distinguishedNameAttr.LdapName, "CN=John Dow,CN=users,DC=testdata,DC=adbook,DC=local");
                    Assert.Throws(typeof(RepositoryInaccessibleError), () => repo.WriteAdPerson(connectionParams, adp));
                }
            }
        }

        [Test]
        public void SkipAttributesNotAllowedToChangeWhenWritingAdPerson()
        {
            // Arrange
            using (var connectionParams = new ConnectionParams())
            using (var repo = new AdRepository())
            using (var ldapRequestBuilder = new LdapRequestBuilder())
            {
                connectionParams.UseCurrentUserCredentials = true;
                connectionParams.UseDomainYouAreLoggedIn = false;
                connectionParams.LdapPath = "LDAP://localhost:55555/CN=users,DC=testdata,DC=adbook,DC=local";
                ldapRequestBuilder.AddRule("cn", LdapRequestBuilder.MatchingRule.ExactMatch, "Kristie Etherton");
                ldapRequestBuilder.AddRule("objectCategory", LdapRequestBuilder.MatchingRule.ExactMatch, "person");
                ldapRequestBuilder.AddAND();

                // Act
                AdPerson adp = null;
                repo.ItemFound += (object sender, ItemFoundEventArgs args) => { adp = args.Adp; };
                var searchTask = repo.StartSearchAsync(connectionParams, ldapRequestBuilder);
                searchTask.Wait();

                using (adp)
                {
                    // disallow the modification of the EmpId attribute
                    adp.SetAttributeWritable(AttrId.EmpId, false);
                    var empIdAttr = AdAttributes.Get(AttrId.EmpId);
                    adp.set_StringAttr(empIdAttr.LdapName, "123456");
                    IEnumerable<AttrId> skippedAttrs = repo.WriteAdPerson(connectionParams, adp);

                    // Assert
                    Assert.IsTrue(skippedAttrs.Contains(AttrId.EmpId));
                }
            }
        }

        [Test]
        public void CanDetermineWhetherSearchIsActive()
        {
            // Arrange
            using (var connectionParams = new ConnectionParams())
            using (var repo = new AdRepository())
            using (var ldapRequestBuilder = new LdapRequestBuilder()) {
                connectionParams.UseCurrentUserCredentials = true;
                connectionParams.UseDomainYouAreLoggedIn = false;
                connectionParams.LdapPath = "LDAP://localhost:55555/CN=users,DC=testdata,DC=adbook,DC=local";
                ldapRequestBuilder.AddRule("objectCategory", LdapRequestBuilder.MatchingRule.ExactMatch, "person");

                bool isSearchActive = false;

                repo.ItemFound += (object sender, ItemFoundEventArgs args) => {
                    args.Adp.Dispose();
                    isSearchActive = repo.IsSearchActive();
                };

                // Act
                var searchTask = repo.StartSearchAsync(connectionParams, ldapRequestBuilder);
                searchTask.Wait();

                // Assert
                Assert.IsTrue(isSearchActive);
            }
        }

        [Test]
        public async Task CanCancelTheSearch()
        {
            // Arrange
            using (var connectionParams = new ConnectionParams())
            using (var repo = new AdRepository())
            using (var ldapRequestBuilder = new LdapRequestBuilder()) {
                connectionParams.UseCurrentUserCredentials = true;
                connectionParams.UseDomainYouAreLoggedIn = false;
                connectionParams.LdapPath = "LDAP://localhost:55555/CN=users,DC=testdata,DC=adbook,DC=local";
                ldapRequestBuilder.AddRule("objectCategory", LdapRequestBuilder.MatchingRule.ExactMatch, "person");

                repo.ItemFound += (object sender, ItemFoundEventArgs args) => {
                    args.Adp.Dispose();
                    repo.CancelSearch();
                };

                // Act
                var searchTask = repo.StartSearchAsync(connectionParams, ldapRequestBuilder);
                try {
                    await searchTask;
                }
                catch(OperationCanceledException ) {

                }
                // Assert
                Assert.IsTrue(searchTask.IsCanceled);
            }
        }

        [Test]
        public void CanStopTheSearch()
        {
            // Arrange
            using (var connectionParams = new ConnectionParams())
            using (var repo = new AdRepository())
            using (var ldapRequestBuilder = new LdapRequestBuilder()) {
                connectionParams.UseCurrentUserCredentials = true;
                connectionParams.UseDomainYouAreLoggedIn = false;
                connectionParams.LdapPath = "LDAP://localhost:55555/CN=users,DC=testdata,DC=adbook,DC=local";
                ldapRequestBuilder.AddRule("objectCategory", LdapRequestBuilder.MatchingRule.ExactMatch, "person");

                repo.ItemFound += (object sender, ItemFoundEventArgs args) => {
                    args.Adp.Dispose();
                    Thread.Sleep(1000);
                };

                // Act
                var searchTask = repo.StartSearchAsync(connectionParams, ldapRequestBuilder);
                repo.EnsureSearchIsStopped();

                // Assert
                Assert.IsTrue(searchTask.IsCanceled);
            }
        }

        [Test]
        public void StartSearchAsyncMethodThrowsExceptionIfConnectionParamsIsNull()
        {
            using (var repo = new AdRepository())
            using (var ldapRequestBuilder = new LdapRequestBuilder()) {

                ldapRequestBuilder.AddRule("objectCategory", LdapRequestBuilder.MatchingRule.ExactMatch, "person");

                // Act
                bool exceptionOccured = false;
                try{
                    var searchTask = repo.StartSearchAsync(null, ldapRequestBuilder);
                    searchTask.Wait();
                }
                catch(ArgumentNullException ) {
                    exceptionOccured = true;
                }

                // Assert
                Assert.IsTrue(exceptionOccured);
            }
        }

        [Test]
        public void StartSearchAsyncMethodThrowsExceptionIfLdapRequestIsNull()
        {
            using (var connectionParams = new ConnectionParams())
            using (var repo = new AdRepository()) {
                connectionParams.UseCurrentUserCredentials = true;
                connectionParams.UseDomainYouAreLoggedIn = false;
                connectionParams.LdapPath = "LDAP://localhost:55555/CN=users,DC=testdata,DC=adbook,DC=local";

                // Act
                bool exceptionOccured = false;
                try {
                    var searchTask = repo.StartSearchAsync(connectionParams, null);
                    searchTask.Wait();
                }
                catch (ArgumentNullException ) {
                    exceptionOccured = true;
                }

                // Assert
                Assert.IsTrue(exceptionOccured);
            }
        }

        [Test]
        public void WriteAdPersonMethodThrowsExceptionIfConnectionParamsIsNull()
        {
            // Arrange
            using (var repo = new AdRepository()) {
                AdPerson adp = new AdPerson();
                // Act
                bool exceptionOccured = false;
                try {
                    repo.WriteAdPerson(null, adp);
                }
                catch (ArgumentNullException) {
                    exceptionOccured = true;
                }
                // Assert
                Assert.IsTrue(exceptionOccured);
            }
        }

        [Test]
        public void WriteAdPersonMethodThrowsExceptionIfAdPersonIsNull()
        {
            // Arrange
            using (var connectionParams = new ConnectionParams())
            using (var repo = new AdRepository()) {
                connectionParams.UseCurrentUserCredentials = true;
                connectionParams.UseDomainYouAreLoggedIn = false;
                connectionParams.LdapPath = "LDAP://localhost:55555/CN=users,DC=testdata,DC=adbook,DC=local";

                // Act
                bool exceptionOccured = false;
                try {
                    repo.WriteAdPerson(connectionParams, null);
                }
                catch (ArgumentNullException) {
                    exceptionOccured = true;
                }
                // Assert
                Assert.IsTrue(exceptionOccured);
            }
        }

        [Test]
        public void ReadAdPersonMethodThrowsExceptionIfDnIsNull()
        {
            // Arrange
            using (var connectionParams = new ConnectionParams())
            using (var repo = new AdRepository()) {
                connectionParams.UseCurrentUserCredentials = true;
                connectionParams.UseDomainYouAreLoggedIn = false;
                connectionParams.LdapPath = "LDAP://localhost:55555/CN=users,DC=testdata,DC=adbook,DC=local";

                // Act
                bool exceptionOccured = false;
                try {
                    repo.ReadAdPerson(connectionParams, null);
                }
                catch (ArgumentNullException) {
                    exceptionOccured = true;
                }
                // Assert
                Assert.IsTrue(exceptionOccured);
            }
        }

        [Test]
        public void ReadAdPersonMethodThrowsExceptionIfConnectionParamsIsNull()
        {
            // Arrange
            using (var connectionParams = new ConnectionParams())
            using (var repo = new AdRepository()) {
                connectionParams.UseCurrentUserCredentials = true;
                connectionParams.UseDomainYouAreLoggedIn = false;
                connectionParams.LdapPath = "LDAP://localhost:55555/CN=users,DC=testdata,DC=adbook,DC=local";

                // Act
                bool exceptionOccured = false;
                try {
                    repo.ReadAdPerson(null, "CN=Kristie Etherton,CN=users,DC=testdata,DC=adbook,DC=local");
                }
                catch (ArgumentNullException) {
                    exceptionOccured = true;
                }
                // Assert
                Assert.IsTrue(exceptionOccured);
            }
        }

        [Test]
        public void IsAvailableMethodThrowsExceptionIfConnectionParamsIsNull()
        {
            // Arrange
            using (var connectionParams = new ConnectionParams())
            using (var repo = new AdRepository()) {
                connectionParams.UseCurrentUserCredentials = true;
                connectionParams.UseDomainYouAreLoggedIn = false;
                connectionParams.LdapPath = "LDAP://localhost:55555/CN=users,DC=testdata,DC=adbook,DC=local";

                // Act
                bool exceptionOccured = false;
                try {
                    repo.IsAvailable(null);
                }
                catch (ArgumentNullException) {
                    exceptionOccured = true;
                }
                // Assert
                Assert.IsTrue(exceptionOccured);
            }
        }
    }
}

