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
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using WpfAdBook.Services;
using adbookcli;
using WpfAdBook.DAL;
using WpfAdBook.ViewModels;
using System.Threading;

namespace WpfAdBookTests
{
    [TestClass]
    public class MainWindowVMTest
    {        
        [TestMethod]
        public void MainWindowVMTest_LoadEntireRepo()
        {
            ISettingsService settingService = new MockSettingsService();
            IDialogService dialogService = new MockDialogService();
            
            using (IRepository repository = new MockRepository(false, false))
            using (IDataService dataService = new DataService(settingService, repository))
            using (var mainWndWM = new MainWindowVM(dataService, dialogService, settingService)) {
                Assert.IsTrue(mainWndWM.StartSearchTaskCommand.CanExecute(null) == true);
                mainWndWM.StartSearchTaskCommand.Execute(null);
                mainWndWM.WaitUntilSearchIsOver();
                Assert.IsTrue(mainWndWM.PersonCollection.Count != 0);
            }
        }

        [TestMethod]
        public void MainWindowVMTest_StartStop()
        {
            ISettingsService settingService = new MockSettingsService();
            IDialogService dialogService = new MockDialogService();

            using (IRepository repository = new MockRepository(false, false))
            using (IDataService dataService = new DataService(settingService, repository))
            using (var mainWndWM = new MainWindowVM(dataService, dialogService, settingService)) {
                Assert.IsTrue(mainWndWM.StartSearchTaskCommand.CanExecute(null) == true);
                mainWndWM.StartSearchTaskCommand.Execute(null);
                Assert.IsTrue(mainWndWM.StartSearchTaskCommand.CanExecute(null) == false);
                Assert.IsTrue(mainWndWM.CancelSearchCommand.CanExecute(null) == true);
                mainWndWM.CancelSearchCommand.Execute(null);
                mainWndWM.WaitUntilSearchIsOver();
                Assert.IsTrue(mainWndWM.PersonCollection.Count != 0);
            }
        }

        [TestMethod]
        public void MainWindowVMTest_AddRemoveSearchFilter()
        {
            ISettingsService settingService = new MockSettingsService();
            IDialogService dialogService = new MockDialogService();

            using (var ldapRequest = new LdapRequestBuilder())
            using (IRepository repository = new MockRepository(false, false))
            using (IDataService dataService = new DataService(settingService, repository))
            using (var mainWndWM = new MainWindowVM(dataService, dialogService, settingService)) {
                mainWndWM.SelectedFilter = new SingleAttrFilterVM(AttrId.CommonName);
                mainWndWM.SelectedMatchingRule = new FilterMatchingRuleVM(LdapRequestBuilder.MatchingRule.BeginWith);
                mainWndWM.CurrentFilterValue = "John";
                mainWndWM.AddFilterCommand.Execute(null);
                Assert.IsTrue(mainWndWM.FilterRules.Count == 1);
                Assert.IsTrue(mainWndWM.FilterRules[0].Filter.Equals(mainWndWM.SelectedFilter));
                Assert.IsTrue(mainWndWM.FilterRules[0].MatchingRule.Equals(mainWndWM.SelectedMatchingRule));
                Assert.IsTrue(mainWndWM.FilterRules[0].Value.Equals(mainWndWM.CurrentFilterValue));
                mainWndWM.SelectedFilterRule = mainWndWM.FilterRules[0];
                mainWndWM.RemoveFilterCommand.Execute(null);
                Assert.IsTrue(mainWndWM.FilterRules.Count == 0); //-V3022
            }
        }
        
        [TestMethod]
        public void MainWindowVMTest_DisplaySettingsWindowCommand()
        {
            ISettingsService settingService = new MockSettingsService();
            IDialogService dialogService = new MockDialogService();

            using (var ldapRequest = new LdapRequestBuilder())
            using (IRepository repository = new MockRepository(false, false))
            using (IDataService dataService = new DataService(settingService, repository))
            using (var mainWndWM = new MainWindowVM(dataService, dialogService, settingService)) {
                Assert.IsTrue(dialogService.IsSettingsWindowVisible() == false); //-V3022
                mainWndWM.DisplaySettingsWindowCommand.Execute(null);
                Assert.IsTrue(dialogService.IsSettingsWindowVisible() == true); //-V3022
            }
        }

        [TestMethod]
        public void MainWindowVMTest_EditCommand()
        {
            ISettingsService settingService = new MockSettingsService();
            IDialogService dialogService = new MockDialogService();

            using (IRepository repository = new MockRepository(false, false))
            using (IDataService dataService = new DataService(settingService, repository))
            using (var mainWndWM = new MainWindowVM(dataService, dialogService, settingService)) {
                Assert.IsTrue(mainWndWM.StartSearchTaskCommand.CanExecute(null) == true);
                mainWndWM.StartSearchTaskCommand.Execute(null);
                mainWndWM.WaitUntilSearchIsOver();
                Assert.IsTrue(mainWndWM.EditCommand.CanExecute(null) == false);
                Assert.IsTrue(mainWndWM.PersonCollection.Count != 0);
                mainWndWM.SelectedPerson = mainWndWM.PersonCollection.First();
                Assert.IsTrue(mainWndWM.EditCommand.CanExecute(null) == true);
                mainWndWM.EditCommand.Execute(null);
                Assert.IsTrue(dialogService.IsPersonEditorVisible() == true);
            }
        }
    }
}
