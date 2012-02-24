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
using System.Windows.Media.Animation;
using ICSharpCode.AvalonEdit.Editing;


namespace UltraDemoInterface
{
    /// <summary>
    /// MainWindow.xaml 的交互逻辑
    /// </summary>
    public partial class MainWindow : Window
    {
        private Point lastMousePos;
        private TranslateTransform lastOutputWindowTranslate;
        private TranslateTransform currOutputWindowTranslate;
        private TranslateTransform lastMemoryWindowTranslate;
        private TranslateTransform currMemoryWindowTranslate;

        private OutputWindow outputWindow;
        private SelectAnimationWindow selectAnimationWindow;

        private EditorAdapter editorAdapter;

        public MainWindow()
        {
            InitializeComponent();
            lastOutputWindowTranslate = new TranslateTransform();
            currOutputWindowTranslate = new TranslateTransform();
            lastMemoryWindowTranslate = new TranslateTransform();
            currMemoryWindowTranslate = new TranslateTransform();

            editorAdapter = new EditorAdapter( textEditor );

            // 初始化其他窗口
            //outputWindow = new OutputWindow();
            selectAnimationWindow = new SelectAnimationWindow();
            //selectAnimationWindow.Show();
            //selectAnimationWindow.Visibility = Visibility.Visible;

            AnimationPluginManager mgr = new AnimationPluginManager(selectAnimationWindow);
            mgr.LoadPlugins("AnimationPlugins");

            
        }

        /// <summary>
        /// 窗口关闭时释放资源
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            if (outputWindow != null)
                outputWindow.Close();
            if (selectAnimationWindow != null)
                selectAnimationWindow.Close();
        }

        /// <summary>
        /// 输出窗口
        /// </summary>
        private void OBackground_MouseMove(object sender, MouseEventArgs e)
        {
            //            <ScaleTransform/>
            //            <SkewTransform/>
            //            <RotateTransform/>
            //            <TranslateTransform/>
            if (e.LeftButton == MouseButtonState.Released)
                return;
            Point curMousePos = e.GetPosition(this);
            currOutputWindowTranslate = new TranslateTransform(curMousePos.X - lastMousePos.X, curMousePos.Y - lastMousePos.Y);
            currOutputWindowTranslate.X += lastOutputWindowTranslate.X;
            currOutputWindowTranslate.Y += lastOutputWindowTranslate.Y;
            TransformGroup group = new TransformGroup();
            group.Children.Add(new ScaleTransform(1, 1, OutputWindow.Width/2.0, OutputWindow.Height/2.0));
            group.Children.Add(new SkewTransform());
            group.Children.Add(new RotateTransform());
            group.Children.Add(currOutputWindowTranslate);
            OutputWindow.RenderTransform = group;
        }
        /// <summary>
        /// ……
        /// </summary>
        private void OBackground_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            lastMousePos = e.GetPosition(this);
        }
        /// <summary>
        /// ……
        /// </summary>
        private void OBackground_MouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            lastOutputWindowTranslate = currOutputWindowTranslate;
        }

        /// <summary>
        /// 内存窗口
        /// </summary>
        private void MBackground_MouseMove(object sender, MouseEventArgs e)
        {
            if (e.LeftButton == MouseButtonState.Released)
                return;
            Point curMousePos = e.GetPosition(this);
            currMemoryWindowTranslate = new TranslateTransform(curMousePos.X - lastMousePos.X, curMousePos.Y - lastMousePos.Y);
            currMemoryWindowTranslate.X += lastMemoryWindowTranslate.X;
            currMemoryWindowTranslate.Y += lastMemoryWindowTranslate.Y;
            TransformGroup group = new TransformGroup();
            group.Children.Add(new ScaleTransform(1, 1, MemoryWindow.Width / 2.0, MemoryWindow.Height / 2.0));
            group.Children.Add(new SkewTransform());
            group.Children.Add(new RotateTransform());
            group.Children.Add(currMemoryWindowTranslate);
            MemoryWindow.RenderTransform = group;
        }
        /// <summary>
        /// ……
        /// </summary>
        private void MBackground_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            lastMousePos = e.GetPosition(this);
        }
        /// <summary>
        /// ……
        /// </summary>
        private void MBackground_MouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            lastMemoryWindowTranslate = currMemoryWindowTranslate;
        }

        /// <summary>
        /// 输出窗口按钮快捷键
        /// </summary>
        private void CommandBinding_OutputWindowButtonHotKey_Executed(object sender, ExecutedRoutedEventArgs e)
        {
            if (EditorViewButton.IsChecked == true)
                return;
            if (OutputWindowButton.IsChecked == false)
            {
                OutputWindowButton.RaiseEvent(new RoutedEventArgs(RadioButton.CheckedEvent));
                //System.Windows.MessageBox.Show(OutputWindowButton.Background.CanFreeze.ToString());
                try { OutputWindowButton.IsChecked = true; }
                catch { }
            }
            else
            {
                OutputWindowButton.RaiseEvent(new RoutedEventArgs(RadioButton.UncheckedEvent));
                //System.Windows.MessageBox.Show(OutputWindowButton.Background.CanFreeze.ToString());
                try { OutputWindowButton.IsChecked = false; }
                catch { }
            }
        }

        /// <summary>
        /// 内存窗口按钮快捷键
        /// </summary>
        private void CommandBinding_MemoryWindowButtonHotKey_Executed(object sender, ExecutedRoutedEventArgs e)
        {
            if (EditorViewButton.IsChecked == true)
                return;
            if (MemoryWindowButton.IsChecked == false)
            {
                MemoryWindowButton.RaiseEvent(new RoutedEventArgs(RadioButton.CheckedEvent));
                try { MemoryWindowButton.IsChecked = true; }
                catch { }
            }
            else
            {
                MemoryWindowButton.RaiseEvent(new RoutedEventArgs(RadioButton.UncheckedEvent));
                try { MemoryWindowButton.IsChecked = false; }
                catch { }
            }
        }

        /// <summary>
        /// 打开动画选择窗口
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void MenuItem_Click(object sender, RoutedEventArgs e)
        {
            DoubleAnimationUsingKeyFrames scaleYAni = new DoubleAnimationUsingKeyFrames();
            CubicEase ef = new CubicEase();
            scaleYAni.KeyFrames.Add(new EasingDoubleKeyFrame(0, KeyTime.FromTimeSpan(TimeSpan.FromSeconds(0)), ef));
            scaleYAni.KeyFrames.Add(new EasingDoubleKeyFrame(selectAnimationWindow.Height, KeyTime.FromTimeSpan(TimeSpan.FromSeconds(0.5)), ef));
            selectAnimationWindow.BeginAnimation(Window.HeightProperty, scaleYAni);
            selectAnimationWindow.Show();
        }

    }
    
}
