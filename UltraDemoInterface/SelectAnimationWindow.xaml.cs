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
        public SelectAnimationWindow()
        {
            InitializeComponent();
            animationInfoMap = new Dictionary<String, AnimationInfo>();
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
            Description.Text = selectedItem.description;
            WatchedList.Items.Clear();
            foreach (String value in selectedItem.watchedList)
            {
                ListBoxItem item = new ListBoxItem();
                item.Content = value;
                item.Template = (ControlTemplate)App.Current.FindResource("ListBoxItemTemplate");
                WatchedList.Items.Add(item);
            }
            //WatchedList
        }

        /// <summary>
        /// 确认应用动画并关闭
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void OK_Click(object sender, RoutedEventArgs e)
        {
            ObjectAnimationUsingKeyFrames visAni = new ObjectAnimationUsingKeyFrames();
            visAni.KeyFrames.Add(new DiscreteObjectKeyFrame(Visibility.Hidden, KeyTime.FromTimeSpan(TimeSpan.FromSeconds(0.5))));
            this.BeginAnimation(Window.VisibilityProperty, visAni);
            DoubleAnimationUsingKeyFrames scaleYAni = new DoubleAnimationUsingKeyFrames();
            CubicEase ef = new CubicEase();
            scaleYAni.KeyFrames.Add(new EasingDoubleKeyFrame(Height, KeyTime.FromTimeSpan(TimeSpan.FromSeconds(0)), ef));
            scaleYAni.KeyFrames.Add(new EasingDoubleKeyFrame(0, KeyTime.FromTimeSpan(TimeSpan.FromSeconds(0.5)), ef));
            this.BeginAnimation(Window.HeightProperty, scaleYAni);
            //Hide();
        }

        /// <summary>
        /// 直接关闭窗口
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void Cancle_Click(object sender, RoutedEventArgs e)
        {
            ObjectAnimationUsingKeyFrames visAni = new ObjectAnimationUsingKeyFrames();
            visAni.KeyFrames.Add(new DiscreteObjectKeyFrame(Visibility.Hidden, KeyTime.FromTimeSpan(TimeSpan.FromSeconds(0.5))));
            this.BeginAnimation(Window.VisibilityProperty, visAni);
            DoubleAnimationUsingKeyFrames scaleYAni = new DoubleAnimationUsingKeyFrames();
            CubicEase ef = new CubicEase();
            scaleYAni.KeyFrames.Add(new EasingDoubleKeyFrame(Height, KeyTime.FromTimeSpan(TimeSpan.FromSeconds(0)), ef));
            scaleYAni.KeyFrames.Add(new EasingDoubleKeyFrame(0, KeyTime.FromTimeSpan(TimeSpan.FromSeconds(0.5)), ef));
            this.BeginAnimation(Window.HeightProperty, scaleYAni);
            //Hide();
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

        public Dictionary<String, AnimationInfo> animationInfoMap;

        ///// <summary>
        ///// 缩放显示窗体动画
        ///// </summary>
        ///// <param name="sender"></param>
        ///// <param name="e"></param>
        //private void Window_IsVisibleChanged(object sender, DependencyPropertyChangedEventArgs e)
        //{
        //    if (IsVisible == false)
        //        return;
        //    DoubleAnimationUsingKeyFrames scaleYAni = new DoubleAnimationUsingKeyFrames();
        //    CubicEase ef = new CubicEase();
        //    scaleYAni.KeyFrames.Add(new EasingDoubleKeyFrame(0, KeyTime.FromTimeSpan(TimeSpan.FromSeconds(0)), ef));
        //    scaleYAni.KeyFrames.Add(new EasingDoubleKeyFrame(Height, KeyTime.FromTimeSpan(TimeSpan.FromSeconds(0.5)), ef));
        //    this.BeginAnimation(Window.HeightProperty, scaleYAni);
        //}
        

        
    }
}
