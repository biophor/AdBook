using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using NUnit;
using NUnit.Framework;
using WpfAdBook.DAL;
using Moq;
using adbookcli;
using WpfAdBook.Services;
using WpfAdBook.ViewModels;

namespace WpfAdBook_UnitTests
{
    [TestFixture]
    public class DataService_Tests
    {
        [Test]
        public void IsAvailableMethodMustReturnTrueIfUnderlyingRepoIsAvailable()
        {
            // Arrange
            var moqSettingsService = new Mock<ISettingsService>();
            ConnectionParams connectionParams = new ConnectionParams();
            moqSettingsService.Setup(ss => ss.ConnectionParams).Returns(connectionParams);
            var moqRepo = new Mock<IRepository>();
            moqRepo.Setup(repo => repo.IsAvailable(moqSettingsService.Object.ConnectionParams)).Returns(true);
            DataService dataService = new DataService(moqSettingsService.Object, moqRepo.Object);

            // Act
            bool dataIsAccessible = dataService.IsDataAccessible();

            // Assert
            Assert.IsTrue(dataIsAccessible);
        }

        [Test]
        public void IsAvailableMethodMustReturnFalseIfUnderlyingRepoIsNotAvailable()
        {
            // Arrange
            var moqSettingsService = new Mock<ISettingsService>();
            ConnectionParams connectionParams = new ConnectionParams();
            moqSettingsService.Setup(ss => ss.ConnectionParams).Returns(connectionParams);
            var moqRepo = new Mock<IRepository>();
            moqRepo.Setup(repo => repo.IsAvailable(moqSettingsService.Object.ConnectionParams)).Returns(false);
            DataService dataService = new DataService(moqSettingsService.Object, moqRepo.Object);

            // Act
            bool dataIsAccessible = dataService.IsDataAccessible();

            // Assert
            Assert.IsFalse(dataIsAccessible);
        }

        [Test]
        public void CheckConnectionParamsMethodMustReturnTrueIfUnderlyingRepoIsAvailable()
        {
            // Arrange
            var moqSettingsService = new Mock<ISettingsService>();
            ConnectionParams connectionParams = new ConnectionParams();
            moqSettingsService.Setup(ss => ss.ConnectionParams).Returns(connectionParams);
            var moqRepo = new Mock<IRepository>();
            moqRepo.Setup(repo => repo.IsAvailable(moqSettingsService.Object.ConnectionParams)).Returns(true);
            DataService dataService = new DataService(moqSettingsService.Object, moqRepo.Object);

            // Act
            bool connectionParamsIsCorrect = dataService.CheckConnectionParams(connectionParams);

            // Assert
            Assert.IsTrue(connectionParamsIsCorrect);
        }

        [Test]
        public void CheckConnectionParamsMethodMustReturnFalseIfUnderlyingRepoIsNotAvailable()
        {
            // Arrange
            var moqSettingsService = new Mock<ISettingsService>();
            ConnectionParams connectionParams = new ConnectionParams();
            moqSettingsService.Setup(ss => ss.ConnectionParams).Returns(connectionParams);
            var moqRepo = new Mock<IRepository>();
            moqRepo.Setup(repo => repo.IsAvailable(moqSettingsService.Object.ConnectionParams)).Returns(false);
            DataService dataService = new DataService(moqSettingsService.Object, moqRepo.Object);

            // Act
            bool connectionParamsIsCorrect = dataService.CheckConnectionParams(connectionParams);

            // Assert
            Assert.IsFalse(connectionParamsIsCorrect);
        }

        [Test]
        public void CheckConnectionParamsMethodMustThrowExceptionIfConnectionParamsIsNull()
        {
            // Arrange
            var moqSettingsService = new Mock<ISettingsService>();
            ConnectionParams connectionParams = new ConnectionParams();
            moqSettingsService.Setup(ss => ss.ConnectionParams).Returns(connectionParams);
            var moqRepo = new Mock<IRepository>();
            moqRepo.Setup(repo => repo.IsAvailable(moqSettingsService.Object.ConnectionParams)).Returns(true);
            DataService dataService = new DataService(moqSettingsService.Object, moqRepo.Object);
            // Act & Assert
            Assert.Throws(typeof(ArgumentNullException), () => dataService.CheckConnectionParams(null));
        }

        [Test]
        public void WriteAdPersonMethodMustBeRedirectedToUnderlyingRepo()
        {
            // Arrange
            AdPersonVM adp = new AdPersonVM();
            var moqSettingsService = new Mock<ISettingsService>();
            ConnectionParams connectionParams = new ConnectionParams();
            moqSettingsService.Setup(ss => ss.ConnectionParams).Returns(connectionParams);
            var moqRepo = new Mock<IRepository>();
            moqRepo.Setup(repo => repo.WriteAdPerson(moqSettingsService.Object.ConnectionParams, adp.GetUnderlyingType()));
            DataService dataService = new DataService(moqSettingsService.Object, moqRepo.Object);

            // Act
            dataService.WriteAdPerson(adp);

            // Assert
            moqRepo.VerifyAll();
        }

        [Test]
        public void ReadAdPersonMethodMustBeRedirectedToUnderlyingRepo()
        {
            // Arrange
            AdPerson adp = new AdPerson();
            var moqSettingsService = new Mock<ISettingsService>();
            ConnectionParams connectionParams = new ConnectionParams();
            moqSettingsService.Setup(ss => ss.ConnectionParams).Returns(connectionParams);
            var moqRepo = new Mock<IRepository>();
            string dn = "cn=somename,dc=domain,dc=local";
            moqRepo.Setup(repo => repo.ReadAdPerson(connectionParams, dn)).Returns(adp);
            DataService dataService = new DataService(moqSettingsService.Object, moqRepo.Object);

            // Act
            dataService.ReadAdPerson(dn);

            // Assert
            moqRepo.VerifyAll();
        }

        [Test]
        public void ReadAdPersonMethodMustMustThrowExceptionIfDnIsNullOrEmpty()
        {
            // Arrange
            AdPerson adp = new AdPerson();
            var moqSettingsService = new Mock<ISettingsService>();
            ConnectionParams connectionParams = new ConnectionParams();
            moqSettingsService.Setup(ss => ss.ConnectionParams).Returns(connectionParams);
            var moqRepo = new Mock<IRepository>();
            string dn = "cn=somename,dc=domain,dc=local";
            moqRepo.Setup(repo => repo.ReadAdPerson(connectionParams, dn)).Returns(adp);
            DataService dataService = new DataService(moqSettingsService.Object, moqRepo.Object);

            // Act & Assert
            Assert.Throws(typeof(ArgumentException), () => dataService.ReadAdPerson(null));
        }

        [Test]
        public void WriteAdPersonMethodMustThrowExceptionIfAdPersonVmIsNull()
        {
            // Arrange
            var moqSettingsService = new Mock<ISettingsService>();
            ConnectionParams connectionParams = new ConnectionParams();
            moqSettingsService.Setup(ss => ss.ConnectionParams).Returns(connectionParams);
            var moqRepo = new Mock<IRepository>();
            DataService dataService = new DataService(moqSettingsService.Object, moqRepo.Object);

            // Act& Assert
            Assert.Throws(typeof(ArgumentNullException), () => dataService.WriteAdPerson(null));
        }

        [Test]
        public void StartSearchMethodMustReturnTaskIfRepoIsAvailable()
        {
            // Arrange
            var ldapRequestBuilder = new LdapRequestBuilder();
            var moqSettingsService = new Mock<ISettingsService>();
            ConnectionParams connectionParams = new ConnectionParams();
            moqSettingsService.SetupGet(ss => ss.ConnectionParams).Returns(connectionParams);

            var moqRepo = new Mock<IRepository>();
            moqRepo.Setup(repo => repo.StartSearchAsync(moqSettingsService.Object.ConnectionParams, ldapRequestBuilder))
                .Returns(Task.CompletedTask);
            DataService dataService = new DataService(moqSettingsService.Object, moqRepo.Object);

            // Act
            Task task = dataService.StartSearchAsync(ldapRequestBuilder);

            // Assert
            Assert.IsNotNull(task);
        }

        [Test]
        public void StartSearchMethodMustThrowExceptionIfldapRequestIsNull()
        {
            // Arrange
            var moqSettingsService = new Mock<ISettingsService>();
            ConnectionParams connectionParams = new ConnectionParams();
            moqSettingsService.SetupGet(ss => ss.ConnectionParams).Returns(connectionParams);

            var moqRepo = new Mock<IRepository>();
            DataService dataService = new DataService(moqSettingsService.Object, moqRepo.Object);

            // Act & Assert
            Assert.Throws(typeof(ArgumentNullException), () => dataService.StartSearchAsync(null));
        }

        [Test]
        public void CancelSearchMethodMustBeRedirectedToTheUnderlyingRepo()
        {
            // Arrange
            var ldapRequestBuilder = new LdapRequestBuilder();
            var moqSettingsService = new Mock<ISettingsService>();
            ConnectionParams connectionParams = new ConnectionParams();
            moqSettingsService.SetupGet(ss => ss.ConnectionParams).Returns(connectionParams);

            var moqRepo = new Mock<IRepository>();

            moqRepo.Setup(
                repo => repo.StartSearchAsync(moqSettingsService.Object.ConnectionParams, ldapRequestBuilder))
                        .Returns(Task.CompletedTask);

            moqRepo.Setup(repo => repo.CancelSearch());

            DataService dataService = new DataService(moqSettingsService.Object, moqRepo.Object);
            Task searchTask = dataService.StartSearchAsync(ldapRequestBuilder);
            // Act
            dataService.CancelSearch();
            searchTask.Wait();

            // Assert
            moqRepo.VerifyAll();
        }
    }
}
