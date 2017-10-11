using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NUnit;
using System.Threading.Tasks;
using NUnit.Framework;
using Moq;
using WpfAdBook;
using WpfAdBook.ViewModels;
using WpfAdBook.Services;
using WpfAdBook.DAL;
using adbookcli;
using System.Collections.ObjectModel;

namespace WpfAdBook_UnitTests
{
    [TestFixture]
    internal class ViewModelLocator_Tests
    {
        [Test]
        public void MainWindowViewModelIsAccessible()
        {
            var mockSettingsService = new Mock<ISettingsService>();
            var filteringParams = new FilteringParams();
            var connectionParams = new ConnectionParams();
            var people = new ObservableCollection<AdPersonVM>();
            mockSettingsService.Setup(ss => ss.FilteringParams).Returns(filteringParams);
            mockSettingsService.Setup(ss => ss.ConnectionParams).Returns(connectionParams);
            mockSettingsService.Setup(ss => ss.SavedSearchResult).Returns(people);
            var mockRepository = new Mock<IRepository>();

            using (var vl = new ViewModelLocator(mockSettingsService.Object, mockRepository.Object)) {
                var mainWindowVM = vl.MainWindowViewModel;
                Assert.IsNotNull(mainWindowVM);
                Assert.IsTrue(mainWindowVM.GetType() == typeof(MainWindowVM));
            }
        }

        [Test]
        public void SettingsWindowViewModelIsAccessible()
        {
            var mockSettingsService = new Mock<ISettingsService>();
            var filteringParams = new FilteringParams();
            var connectionParams = new ConnectionParams();
            var people = new ObservableCollection<AdPersonVM>();
            mockSettingsService.Setup(ss => ss.FilteringParams).Returns(filteringParams);
            mockSettingsService.Setup(ss => ss.ConnectionParams).Returns(connectionParams);
            mockSettingsService.Setup(ss => ss.SavedSearchResult).Returns(people);
            var mockRepository = new Mock<IRepository>();

            using (var vl = new ViewModelLocator(mockSettingsService.Object, mockRepository.Object)) {
                var settingsWindowVM = vl.SettingsWindowViewModel;
                Assert.IsNotNull(settingsWindowVM);
                Assert.IsTrue(settingsWindowVM.GetType() == typeof(SettingsWindowVM));
            }
        }

        [Test]
        public void EditPersonWindowViewModelIsAccessible()
        {
            var mockSettingsService = new Mock<ISettingsService>();
            var filteringParams = new FilteringParams();
            var connectionParams = new ConnectionParams();
            var people = new ObservableCollection<AdPersonVM>();
            mockSettingsService.Setup(ss => ss.FilteringParams).Returns(filteringParams);
            mockSettingsService.Setup(ss => ss.ConnectionParams).Returns(connectionParams);
            mockSettingsService.Setup(ss => ss.SavedSearchResult).Returns(people);
            var mockRepository = new Mock<IRepository>();

            using (var vl = new ViewModelLocator(mockSettingsService.Object, mockRepository.Object)) {
                var editPersonWindowVM = vl.EditPersonWindowViewModel;
                Assert.IsNotNull(editPersonWindowVM);
                Assert.IsTrue(editPersonWindowVM.GetType() == typeof(EditPersonWindowVM));
            }
        }
    }
}
