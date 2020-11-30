using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WpfAdBook.Services
{
    public class MockDialogService : IDialogService
    {
        bool _personEditorVisible = false;
        bool _settingsWindowVisible = false;

        public void CloseEditPersonWindow()
        {
            _personEditorVisible = false;
        }

        public void CloseMainWindow()
        {
            Debug.WriteLine("called CloseMainWindow()");
        }

        public void SaveMainWindowState()
        {
            Debug.WriteLine("called SaveMainWindowState()");
        }

        public void RestoreMainWindowState()
        {
            Debug.WriteLine("called RestoreMainWindowState()");
        }

        public void CloseSettingsWindow()
        {
            _settingsWindowVisible = false;
        }

        public void DisplayEditPersonWindow()
        {
            _personEditorVisible = true;
        }

        public void DisplayErrorMessage(string errorMessage)
        {
            Debug.WriteLine($"Called DisplayErrorMessage({errorMessage})");
        }

        public void DisplaySuccessMessage(string successMessage)
        {
            Debug.WriteLine($"Called DisplaySuccessMessage({successMessage})");
        }

        public void DisplaySettingsWindow()
        {
            _settingsWindowVisible = true;
        }

        public string SelectPhoto()
        {
            Debug.WriteLine("called SelectPhoto()");
            return "c:\\photo.jpg";
        }

        public bool IsSettingsWindowVisible()
        {
            return _settingsWindowVisible;
        }

        public bool IsPersonEditorVisible()
        {
            return _personEditorVisible;
        }

        public void DisplayAboutBox()
        {
            Debug.WriteLine("called DisplayAboutBox()");
        }

        public bool RequestConfirmationOnAppExit()
        {
            Debug.WriteLine("called RequestConfirmationOnAppExit()");
            return true;
        }


        public void SaveEditPersonWindowState()
        {

        }

        public void RestoreEditPersonWindowState()
        {

        }
    }
}
