using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using WpfAdBook.ViewModels;

namespace WpfAdBook.Controls
{
    /// <summary>
    /// Interaction logic for PersonDetailsControl.xaml
    /// </summary>
    public partial class PersonDetailsControl : UserControl
    {
        public PersonDetailsControl()
        {
            InitializeComponent();
            (Content as FrameworkElement).DataContext = this;
        }

        public AdPersonVM SelectedPerson {
            get {
                return (AdPersonVM)GetValue(SelectedPersonProperty);
            }
            set {
                SetValueDp(SelectedPersonProperty, value);
            }
        }

        // Using a DependencyProperty as the backing store for SelectedPerson.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty SelectedPersonProperty =
            DependencyProperty.Register(
                "SelectedPerson",
                typeof(AdPersonVM),
                typeof(PersonDetailsControl),
                new PropertyMetadata()
                );



        public event PropertyChangedEventHandler PropertyChanged;

        private void SetValueDp(DependencyProperty property, object value, [CallerMemberName] string p = null)
        {
            SetValue(property, value);
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(p));
        }
    }
}
