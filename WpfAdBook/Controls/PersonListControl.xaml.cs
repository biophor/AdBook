using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Runtime.CompilerServices;
using System.Windows;
using System.Windows.Controls;
using WpfAdBook.ViewModels;

namespace WpfAdBook.Controls
{
    /// <summary>
    /// Interaction logic for PersonListControl.xaml
    /// </summary>
    internal partial class PersonListControl : UserControl
    {
        public PersonListControl()
        {
            InitializeComponent();
            (Content as FrameworkElement).DataContext = this;
        }

        internal AdPersonVM SelectedPerson {
            get { return (AdPersonVM)GetValue(SelectedPersonProperty); }
            set { SetValueDp(SelectedPersonProperty, value); }
        }

        internal static readonly DependencyProperty SelectedPersonProperty =
            DependencyProperty.Register(
                "SelectedPerson",
                typeof(AdPersonVM),
                typeof(PersonListControl),
                new PropertyMetadata()
                );



        internal static readonly DependencyProperty PersonListSourceProperty =
            DependencyProperty.Register(
                "PersonListSource",
                typeof(ObservableCollection<AdPersonVM>),
                typeof(PersonListControl),
                new PropertyMetadata(new ObservableCollection<AdPersonVM>())
                );

        internal ObservableCollection<AdPersonVM> PersonListSource {
            get { return (ObservableCollection<AdPersonVM>)GetValue(PersonListSourceProperty); }
            set { SetValueDp(PersonListSourceProperty, value); }
        }

        internal event PropertyChangedEventHandler PropertyChanged;

        private void SetValueDp(DependencyProperty property, object value, [CallerMemberName] string p = null)
        {
            SetValue(property, value);
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(p));
        }
    }
}