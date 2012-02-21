using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Data;

/**
@date		:	2012/02/21
@filename	: 	NumericalConverter.cs
@author		:	屠文翔	
			 -     _     - 
			| | - (_) __| |
			| |/ /| |/ _  |
			|   < | |||_| |
			|_|\_\|_|\____|
@note		:	在XAML中提供简单的数值绑定转换。
**/

namespace UltraDemoInterface
{
    class NumericalConverter : IValueConverter
    {
        public System.Object Convert(System.Object value, System.Type targetType, System.Object parameter, System.Globalization.CultureInfo culture)
        {
            if (value == null)
                return value;
            float v = float.Parse(value.ToString());
            string parm = (string)parameter;
            string op = null;
            float num = float.NaN;
            if (parm != null)
            {
                string[] parms = parm.Split(",".ToCharArray(), 2);
                op = parms[0];
                if (parms.Length > 1)
                    num = float.Parse(parms[1]);
                if (float.IsNaN(num))
                    return value;
                switch (op)
                {
                    case "+": value = v + num; break;
                    case "-": value = v - num; break;
                    case "*": value = (v * num).ToString(); break;
                    case "/": value = v / num; break;
                    case "P": value = Math.Pow(v, num); break;
                    case "L": value = Math.Log(v, num); break;
                }
                //System.Windows.MessageBox.Show(value.ToString());
                return value;
            }
            return value;
        }

        public System.Object ConvertBack(System.Object value, System.Type targetType, System.Object parameter, System.Globalization.CultureInfo culture)
        {
            return Convert(value, targetType, parameter, culture);
        }
    }
}
