using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Linq;
using System.Threading.Tasks;
using System.Windows;
using WpfAdBook.ViewModels;

namespace WpfAdBook
{
    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : Application
    {
        private App()
        {
            //System.Threading.Thread.CurrentThread.CurrentUICulture = new System.Globalization.CultureInfo("ru-RU");
            System.Threading.Thread.CurrentThread.CurrentUICulture = System.Threading.Thread.CurrentThread.CurrentCulture;
            InitializeComponent();
        }

        [STAThread]
        static void Main()
        {
            App app = new App();

            using (var vml = (ViewModelLocator)app.Resources["globalViewModelLocator"]) {
                MainWindow window = new MainWindow();
                app.Run(window);
            }
        }
    }
}
