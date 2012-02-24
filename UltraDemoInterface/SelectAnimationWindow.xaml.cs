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
using System.Windows.Shapes;
using System.Windows.Media.Animation;

namespace UltraDemoInterface
{
    /// <summary>
    /// SelectAnimationWindow.xaml 的交互逻辑
    /// </summary>
    public partial class SelectAnimationWindow : Window
    {
        public SelectAnimationWindow(MainWindow mainWindow)
        {
            InitializeComponent();
            animationInfoMap = new Dictionary<String, AnimationInfo>();
            this.mainWindow = mainWindow;
        }

        /// <summary>
        /// 使鼠标拖动窗口
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void Grid_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            DragMove();
        }

        /// <summary>
        /// 用户在动画列表中选中相应动画项
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void AnimationList_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (AnimationList.SelectedItem == null)
                return;
            AnimationInfo selectedItem = animationInfoMap[(AnimationList.SelectedItem as ListBoxItem).Content.ToString()];
            // 更新description
            Description.Text = selectedItem.description;
            // 更新watched list
            WatchedList.Items.Clear();
            foreach (String value in selectedItem.watchedList)
            {
                ListBoxItem item = new ListBoxItem();
                item.Content = value;
                item.Template = (ControlTemplate)App.Current.FindResource("ListBoxItemTemplate");
                WatchedList.Items.Add(item);
            }
        }

        /// <summary>
        /// 确认应用动画并关闭
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void OK_Click(object sender, RoutedEventArgs e)
        {
            if (isWindowFading == true)
                return;
            isWindowFading = true;
            mainWindow.HideWindow(this);
        }

        /// <summary>
        /// 直接关闭窗口
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void Cancle_Click(object sender, RoutedEventArgs e)
        {
            if (isWindowFading == true)
                return;
            isWindowFading = true;
            mainWindow.HideWindow(this);
        }

        /// <summary>
        /// 结构，用以存放动画信息
        /// </summary>
        public struct AnimationInfo
        {
            //public String name;
            //public Bitmap preview;
            public String description;
            public List<String> watchedList;
        }

        /// <summary>
        /// 禁用Alt+F4
        /// </summary>
        #region Disable_Alt_F4
        private bool AltDown = false;
        private void Window_PreviewKeyUp(object sender, KeyEventArgs e)
        {
            if (e.SystemKey == Key.LeftAlt || e.SystemKey == Key.RightAlt)
                AltDown = false;
        }
        private void Window_PreviewKeyDown(object sender, KeyEventArgs e)
        {
            if (e.SystemKey == Key.LeftAlt || e.SystemKey == Key.RightAlt)
            {
                AltDown = true;
            }
            else if (e.SystemKey == Key.F4 && AltDown)
                e.Handled = true;
        }
        #endregion

        public Dictionary<String, AnimationInfo> animationInfoMap;
        private MainWindow mainWindow;
        public Boolean isWindowFading;
        
    }
}
