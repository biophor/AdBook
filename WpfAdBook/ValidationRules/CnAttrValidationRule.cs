using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Controls;

namespace WpfAdBook.ValidationRules
{
    class CnAttrValidationRule : ValidationRule
    {
        public override ValidationResult Validate(object value, CultureInfo cultureInfo) {
            if (((string)value).Length == 0) {
                return new ValidationResult(
                    false,
                    Properties.Resources.YouShouldProvideValue
                    );
            }
            return new ValidationResult(true, null);
        }
    }
}
