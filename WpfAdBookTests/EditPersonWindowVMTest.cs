using System;
using System.Text;
using System.Collections.Generic;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using WpfAdBook.Services;
using WpfAdBook.DAL;
using WpfAdBook.ViewModels;

namespace WpfAdBookTests
{
    /// <summary>
    /// Summary description for EditPersonWindowVMTest
    /// </summary>
    [TestClass]
    public class EditPersonWindowVMTest
    {
        public EditPersonWindowVMTest()
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

        #region Additional test attributes
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
        // [TestInitialize()]
        // public void MyTestInitialize() { }
        //
        // Use TestCleanup to run code after each test has run
        // [TestCleanup()]
        // public void MyTestCleanup() { }
        //
        #endregion

        [TestMethod]
        public void EditPersonWindowVMTest_SelectedPerson()
        {
            ISettingsService settingService = new MockSettingsService();
            IDialogService dialogService = new MockDialogService();

            using (IRepository repository = new MockRepository(false, false))
            using (IDataService dataService = new DataService(settingService, repository))
            using (var editWndWM = new EditPersonWindowVM(dataService, dialogService)) {
                using (var v = new AdPersonVM()) {
                    v.SetStringAttr(adbookcli.AttrId.CommonName, "John");
                    editWndWM.SelectedPerson = v;
                }
            }
        }
    }
}
