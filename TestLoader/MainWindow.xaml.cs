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
        [DllImport("Compiler.dll", EntryPoint="compile", CharSet=CharSet.Unicode)]
        public static extern string compile([MarshalAs(UnmanagedType.LPWStr)] string code);

        public MainWindow()
        {
            InitializeComponent();
            try
            {
                System.Windows.MessageBox.Show( compile( "Hello" ).ToString() );
            }
            catch (System.Exception ex)
            {
                System.Windows.MessageBox.Show( ex.Message.ToString() );
            }
        }
    }
}
