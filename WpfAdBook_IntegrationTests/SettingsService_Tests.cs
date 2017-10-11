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
using Moq;
using NUnit.Framework;
using NUnit;
using WpfAdBook.Services;
using System.Security;
using WpfAdBook.ViewModels;
using System.Diagnostics;

namespace WpfAdBook_IntegrationTests
{
    [TestFixture]
    class SettingsService_Tests
    {
        [Test]
        public void CanSaveAndLoadConnectionParams()
        {
            using (SettingsService service = new SettingsService(false)) {
                service.ConnectionParams.LdapPath = "LDAP://localhost:55555/cn=FAKE,dc=settings,dc=local";
                service.ConnectionParams = new adbookcli.ConnectionParams();
                service.ConnectionParams.Dc = "somedc1.somedomain.com";
                service.ConnectionParams.UseCurrentUserCredentials = false;
                service.ConnectionParams.Login = "user";
                SecureString password = new SecureString();
                password.AppendChar('p');
                password.AppendChar('a');
                password.AppendChar('s');
                password.AppendChar('s');
                password.AppendChar('w');
                password.AppendChar('o');
                password.AppendChar('r');
                password.AppendChar('d');
                service.ConnectionParams.Password = password;

                Assert.DoesNotThrow(() => service.Save());
            }

            using (SettingsService service = new SettingsService(false)) {
                Assert.DoesNotThrow(() => service.Load());
                string plainPassword = SettingsService.SecureStringToPlainString(service.ConnectionParams.Password);
                Assert.IsTrue(plainPassword == "password");
            }
        }

        [Test]
        public void CanSaveAndLoadSearchResults()
        {
            using (SettingsService service = new SettingsService(false)) {
                AdPersonVM adp1 = new AdPersonVM();
                adp1.SetStringAttr(adbookcli.AttrId.CommonName, "john");
                service.SavedSearchResult.Add(adp1);
                AdPersonVM adp2 = new AdPersonVM();
                adp2.SetStringAttr(adbookcli.AttrId.CommonName, "mike");
                service.SavedSearchResult.Add(adp2);
                AdPersonVM adp3 = new AdPersonVM();
                adp3.SetStringAttr(adbookcli.AttrId.CommonName, "allice");
                service.SavedSearchResult.Add(adp3);

                Assert.DoesNotThrow(() => service.Save());
                adp1.Dispose();
                adp2.Dispose();
                adp3.Dispose();
            }

            using (SettingsService service = new SettingsService(false)) {

                Assert.DoesNotThrow( () => service.Load());

                Assert.DoesNotThrow(
                    () => service.SavedSearchResult.First(adp => adp.GetStringAttr(adbookcli.AttrId.CommonName) == "john")
                );
                Assert.DoesNotThrow(
                    () => service.SavedSearchResult.First(adp => adp.GetStringAttr(adbookcli.AttrId.CommonName) == "mike")
                );
                Assert.DoesNotThrow(
                    () => service.SavedSearchResult.First(adp => adp.GetStringAttr(adbookcli.AttrId.CommonName) == "allice")
                );
                foreach(var adp in service.SavedSearchResult) {
                    adp.Dispose();
                }
            }
        }

        [Test]
        public void CanSaveAndLoadWindowPlacement()
        {
            using (SettingsService service = new SettingsService(false)) {
                service.MainWndPos = new System.Drawing.Point(123, 456);
                service.MainWndSize = new System.Drawing.Size(321, 654);
                service.MainWndMaximized = false;

                Assert.DoesNotThrow( () => service.Save());
            }
            using (SettingsService service = new SettingsService(false)) {
                Assert.DoesNotThrow(() => service.Load());
                Assert.IsTrue(service.MainWndMaximized == false);
                Assert.IsTrue(service.MainWndPos.X == 123);
                Assert.IsTrue(service.MainWndPos.Y == 456);

                Assert.IsTrue(service.MainWndSize.Width == 321);
                Assert.IsTrue(service.MainWndSize.Height == 654);
            }
        }
    }
}
