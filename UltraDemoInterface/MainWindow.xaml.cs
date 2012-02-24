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

using System.Reflection;


namespace UltraDemoInterface
{
    /// <summary>
    /// MainWindow.xaml 的交互逻辑
    /// </summary>
    public partial class MainWindow : Window
    {

        public OutputWindow outputWindow;
        public MemoryWindow memoryWindow;
        public SelectAnimationWindow selectAnimationWindow;
        public AnimationPluginManager animationPluginManager;
        public DebugControler debugControler;
        public Grid animationContainer;

        //MethodInfo beginRender;
        //Object anima;

        //Rectangle fuckr;

        private EditorAdapter editorAdapter;

        public MainWindow()
        {
            InitializeComponent();

            editorAdapter = new EditorAdapter( textEditor );

            // 初始化其他窗口
            outputWindow = new OutputWindow(this);
            memoryWindow = new MemoryWindow(this);
            selectAnimationWindow = new SelectAnimationWindow(this);
            // 初始化动画插件管理类
            animationContainer = (Grid)FindName("AnimationContainer");
            animationPluginManager = new AnimationPluginManager(this);
            animationPluginManager.LoadPlugins("AnimationPlugins");
            // 初始化debug控制器
            debugControler = new DebugControler(this);

            // 连接回调函数
            CompositionTarget.Rendering += animationPluginManager.RenderAnimationCallback;
            CompositionTarget.Rendering += debugControler.DebugControlerCallback;
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
            if (memoryWindow != null)
                memoryWindow.Close();
            if (selectAnimationWindow != null)
                selectAnimationWindow.Close();
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
        /// 显示窗口
        /// </summary>
        /// <param name="window">要显示的窗口</param>
        public void ShowWindow(Window window)
        {
            ObjectAnimationUsingKeyFrames visAni = new ObjectAnimationUsingKeyFrames();
            visAni.KeyFrames.Add(new DiscreteObjectKeyFrame(Visibility.Visible, KeyTime.FromTimeSpan(TimeSpan.FromSeconds(0))));
            window.BeginAnimation(Window.VisibilityProperty, visAni);

            DoubleAnimationUsingKeyFrames alphaAni = new DoubleAnimationUsingKeyFrames();
            CubicEase ef = new CubicEase();
            alphaAni.KeyFrames.Add(new EasingDoubleKeyFrame(0, KeyTime.FromTimeSpan(TimeSpan.FromSeconds(0)), ef));
            alphaAni.KeyFrames.Add(new EasingDoubleKeyFrame(1, KeyTime.FromTimeSpan(TimeSpan.FromSeconds(0.3)), ef));
            window.BeginAnimation(Window.OpacityProperty, alphaAni);
        }

        /// <summary>
        /// 隐藏窗口
        /// </summary>
        /// <param name="window">要隐藏的窗口</param>
        public void HideWindow(Window window)
        {
            ObjectAnimationUsingKeyFrames visAni = new ObjectAnimationUsingKeyFrames();
            visAni.KeyFrames.Add(new DiscreteObjectKeyFrame(Visibility.Hidden, KeyTime.FromTimeSpan(TimeSpan.FromSeconds(0.3))));
            window.BeginAnimation(Window.VisibilityProperty, visAni);

            DoubleAnimationUsingKeyFrames alphaAni = new DoubleAnimationUsingKeyFrames();
            CubicEase ef = new CubicEase();
            alphaAni.KeyFrames.Add(new EasingDoubleKeyFrame(1, KeyTime.FromTimeSpan(TimeSpan.FromSeconds(0)), ef));
            alphaAni.KeyFrames.Add(new EasingDoubleKeyFrame(0, KeyTime.FromTimeSpan(TimeSpan.FromSeconds(0.3)), ef));
            window.BeginAnimation(Window.OpacityProperty, alphaAni);
        }

        /// <summary>
        /// 显示动画选择窗口
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void MenuItem_Click(object sender, RoutedEventArgs e)
        {
            selectAnimationWindow.isWindowFading = false;
            ShowWindow(selectAnimationWindow);
        }

        /// <summary>
        /// 显示输出窗口
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void OutputWindowButton_Checked(object sender, RoutedEventArgs e)
        {
            ShowWindow(outputWindow);
        }

        /// <summary>
        /// 隐藏输出窗口
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void OutputWindowButton_Unchecked(object sender, RoutedEventArgs e)
        {
            HideWindow(outputWindow);
        }

        /// <summary>
        /// 显示内存窗口
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void MemoryWindowButton_Checked(object sender, RoutedEventArgs e)
        {
            ShowWindow(memoryWindow);
        }

        /// <summary>
        /// 隐藏内存窗口
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void MemoryWindowButton_Unchecked(object sender, RoutedEventArgs e)
        {
            HideWindow(memoryWindow);
        }

    }
    
}
