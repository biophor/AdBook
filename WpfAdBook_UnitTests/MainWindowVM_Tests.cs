using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using NUnit;
using Moq;
using NUnit.Framework;
using WpfAdBook.Services;
using adbookcli;
using System.Collections.ObjectModel;
using WpfAdBook.DAL;
using WpfAdBook.ViewModels;

namespace WpfAdBook_UnitTests
{
    [TestFixture]
    public class MainWindowVM_Tests
    {
        [Test]
        public void Test1()
        {
            var mockSettingsService = new Mock<ISettingsService>();
            var filteringParams = new FilteringParams();
            var connectionParams = new ConnectionParams();
            var people = new ObservableCollection<AdPersonVM>();
            mockSettingsService.Setup(ss => ss.FilteringParams).Returns(filteringParams);
            mockSettingsService.Setup(ss => ss.ConnectionParams).Returns(connectionParams);
            mockSettingsService.Setup(ss => ss.SavedSearchResult).Returns(people);
            var mockRepository = new Mock<IRepository>();
            var mockDialogService = new Mock<IDialogService>();
            var mockDataService = new Mock<IDataService>();

            using (var sut = new MainWindowVM(mockDataService.Object, mockDialogService.Object, mockSettingsService.Object)) {


            }
        }
    }
}
