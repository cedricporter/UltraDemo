using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Runtime.InteropServices;

namespace TestLoader
{
    /// <summary>
    /// MainWindow.xaml 的交互逻辑
    /// </summary>
    public partial class MainWindow : Window
    {
        [DllImport("Compiler.dll", EntryPoint="compile", CharSet=CharSet.Ansi)]
        public static extern int compile(string code, StringBuilder ret);

        public MainWindow()
        {
            InitializeComponent();
            try
            {
                string code = "ET";
                StringBuilder ret = new StringBuilder(50000);
                ret.Append( "ET, ET" );
                compile(code, ret);
                System.Windows.MessageBox.Show( ret.ToString() );
            }
            catch (System.Exception ex)
            {
                System.Windows.MessageBox.Show( ex.Message.ToString() );
            }
        }
    }
}
