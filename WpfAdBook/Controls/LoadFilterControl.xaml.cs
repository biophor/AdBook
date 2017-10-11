using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Runtime.CompilerServices;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using WpfAdBook.ViewModels;

namespace WpfAdBook.Controls
{
    public partial class LoadFilterControl : UserControl
    {
        public LoadFilterControl()
        {
            InitializeComponent();
            (Content as FrameworkElement).DataContext = this;
        }

        public int PredefinedFilterList {
            get { return (int)GetValue(PredefinedFilterListProperty); }
            set { SetValueDp(PredefinedFilterListProperty, value); }
        }

        public static readonly DependencyProperty PredefinedFilterListProperty =
            DependencyProperty.Register(
                "PredefinedFilterList",
                typeof(PredefinedFilterListVM),
                typeof(LoadFilterControl), new PropertyMetadata()
                );

        public FilterVM SelectedFilter {
            get { return (FilterVM)GetValue(SelectedFilterProperty); }
            set { SetValueDp(SelectedFilterProperty, value); }
        }

        public static readonly DependencyProperty SelectedFilterProperty =
            DependencyProperty.Register(
                "SelectedFilter",
                typeof(FilterVM),
                typeof(LoadFilterControl),
                new PropertyMetadata()
                );

        public int PredefinedMatchingRules {
            get { return (int)GetValue(PredefinedMatchingRulesProperty); }
            set { SetValueDp(PredefinedMatchingRulesProperty, value); }
        }

        public static readonly DependencyProperty PredefinedMatchingRulesProperty =
            DependencyProperty.Register(
                "PredefinedMatchingRules",
                typeof(PredefinedMatchingRulesVM),
                typeof(LoadFilterControl),
                new PropertyMetadata()
                );

        public FilterMatchingRuleVM SelectedMatchingRule {
            get { return (FilterMatchingRuleVM)GetValue(SelectedMatchingRuleProperty); }
            set { SetValueDp(SelectedMatchingRuleProperty, value); }
        }

        // Using a DependencyProperty as the backing store for SelectedMatchingRule.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty SelectedMatchingRuleProperty =
            DependencyProperty.Register(
                "SelectedMatchingRule",
                typeof(FilterMatchingRuleVM),
                typeof(LoadFilterControl),
                new PropertyMetadata()
                );

        public string CurrentFilterValue {
            get { return (string)GetValue(CurrentFilterValueProperty); }
            set { SetValueDp(CurrentFilterValueProperty, value); }
        }

        // Using a DependencyProperty as the backing store for CurrentFilterValue.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty CurrentFilterValueProperty =
            DependencyProperty.Register(
                "CurrentFilterValue",
                typeof(string),
                typeof(LoadFilterControl),
                new PropertyMetadata("")
                );

        public ICommand AddFilterCommand {
            get { return (ICommand)GetValue(AddFilterCommandProperty); }
            set { SetValueDp(AddFilterCommandProperty, value); }
        }

        // Using a DependencyProperty as the backing store for AddFilterCommand.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty AddFilterCommandProperty =
            DependencyProperty.Register(
                "AddFilterCommand",
                typeof(ICommand),
                typeof(LoadFilterControl),
                new PropertyMetadata()
                );

        public ICommand RemoveFilterCommand {
            get { return (ICommand)GetValue(RemoveFilterCommandProperty); }
            set { SetValueDp(RemoveFilterCommandProperty, value); }
        }

        // Using a DependencyProperty as the backing store for RemoveFilterCommand.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty RemoveFilterCommandProperty =
            DependencyProperty.Register(
                "RemoveFilterCommand",
                typeof(ICommand),
                typeof(LoadFilterControl),
                new PropertyMetadata()
                );

        public bool AllFilterRulesShouldBeMet {
            get { return (bool)GetValue(AllFilterRulesShouldBeMetProperty); }
            set { SetValueDp(AllFilterRulesShouldBeMetProperty, value); }
        }

        // Using a DependencyProperty as the backing store for AllFilterRulesShouldBeMet.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty AllFilterRulesShouldBeMetProperty =
            DependencyProperty.Register(
                "AllFilterRulesShouldBeMet",
                typeof(bool),
                typeof(LoadFilterControl),
                new PropertyMetadata(false)
                );

        public FilterRuleVM SelectedFilterRule {
            get { return (FilterRuleVM)GetValue(SelectedFilterRuleProperty); }
            set { SetValueDp(SelectedFilterRuleProperty, value); }
        }

        // Using a DependencyProperty as the backing store for SelectedFilterRule.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty SelectedFilterRuleProperty =
            DependencyProperty.Register(
                "SelectedFilterRule",
                typeof(FilterRuleVM),
                typeof(LoadFilterControl),
                new PropertyMetadata()
                );

        public ObservableCollection<FilterRuleVM> FilterRules {
            get { return (ObservableCollection<FilterRuleVM>)GetValue(FilterRulesProperty); }
            set { SetValueDp(FilterRulesProperty, value); }
        }

        // Using a DependencyProperty as the backing store for FilterRules.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty FilterRulesProperty =
            DependencyProperty.Register(
                "FilterRules",
                typeof(ObservableCollection<FilterRuleVM>),
                typeof(LoadFilterControl),
                new PropertyMetadata(new ObservableCollection<FilterRuleVM>())
                );

        public event PropertyChangedEventHandler PropertyChanged;

        private void SetValueDp(DependencyProperty property, object value, [CallerMemberName] string p = null)
        {
            SetValue(property, value);
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(p));
        }
    }
}
