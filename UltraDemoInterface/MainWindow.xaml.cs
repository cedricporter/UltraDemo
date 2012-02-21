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

namespace UltraDemoInterface
{
    /// <summary>
    /// MainWindow.xaml 的交互逻辑
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }

        /*/// <summary>
        /// 显示编辑面板，隐藏演示面板。
        /// 作者：屠文翔
        /// </summary>
        private void EditorViewButton_Checked(object sender, RoutedEventArgs e)
        {
            if (this.IsInitialized)
            {
                EditorContainer.Visibility = Visibility.Visible;
                DemoContainer.Visibility = Visibility.Hidden;
            }
        }

        /// <summary>
        /// 显示演示面板，隐藏编辑面板。
        /// 作者：屠文翔
        /// </summary>
        private void DemoViewButton_Checked(object sender, RoutedEventArgs e)
        {
            if (this.IsInitialized)
            {
                EditorContainer.Visibility = Visibility.Hidden;
                DemoContainer.Visibility = Visibility.Visible;
            }
        }

        /// <summary>
        /// 切换至演示面板动画。
        /// 作者：屠文翔
        /// </summary>
        private void DemoViewButton_Click(object sender, RoutedEventArgs e)
        {
            
        }*/
    }
    
}
