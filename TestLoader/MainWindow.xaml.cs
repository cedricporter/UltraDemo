using System;
using System.Diagnostics;
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
        ETController controller = new ETController();

        public MainWindow()
        {
            InitializeComponent();

            System.IO.StreamReader sr = new System.IO.StreamReader("SAMPLE.hpp");
            controller.Initilialize_Machine( sr.ReadToEnd() );
            try
            {
                string outString = "";
                while (controller.Step())
                {
                    //System.Diagnostics.Debug.WriteLine( controller.GetCurrentLine().ToString() );
                }
                System.Windows.MessageBox.Show( outString );
            }
            catch (System.Exception ex)
            {
                System.Windows.MessageBox.Show( ex.Message.ToString() );
            }
        }
    }
}
