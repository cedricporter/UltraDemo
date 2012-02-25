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
        public ETController etController;
        public Grid animationContainer;
        public EditorAdapter editorAdapter;

        Boolean isRunning = false;

        //MethodInfo beginRender;
        //Object anima;

        //Rectangle fuckr;

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
            // 初始化控制器
            debugControler = new DebugControler(this);
            etController = new ETController();

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
        private void MenuSelectAnimation_Click(object sender, RoutedEventArgs e)
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

        /// <summary>
        /// 运行/从暂停恢复
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void RunButton_Checked(object sender, RoutedEventArgs e)
        {
            // 运行
            if (isRunning == false)
            {
                isRunning = true;
                animationPluginManager.BeginRender();
                debugControler.Run();
                StepButton.IsEnabled = false;
            }
            // 从暂停恢复
            else
            {
                debugControler.Run();
                StepButton.IsEnabled = false;
            }
        }

        /// <summary>
        /// 暂停
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void RunButton_Unchecked(object sender, RoutedEventArgs e)
        {
            debugControler.Pause();
            StepButton.IsEnabled = true;
        }

        /// <summary>
        /// 单步
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void StepButton_Click(object sender, RoutedEventArgs e)
        {
            debugControler.Step();
        }

        /// <summary>
        /// 停止
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void StopButton_Click(object sender, RoutedEventArgs e)
        {
            isRunning = false;
            debugControler.Stop();
            animationPluginManager.StopRender();
            StepButton.IsEnabled = true;
            RunButton.IsChecked = false;
        }

        /// <summary>
        /// 调节代码运行速度
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void Slider_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            Double timeInterval = SpeedControlSlider.Maximum - e.NewValue;
            debugControler.SetTimeInterval(timeInterval);
            Canvas.SetLeft(TimeIntervalText, e.NewValue / 2.42);
            TimeIntervalText.Text = ((int)timeInterval).ToString();
        }
        /// <summary>
        /// 显示Slider的数值提示
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void SpeedControlSlider_MouseEnter(object sender, MouseEventArgs e)
        {
            TimeIntervalText.Visibility = Visibility.Visible;
        }
        /// <summary>
        /// 隐藏Slider的数值提示
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void SpeedControlSlider_MouseLeave(object sender, MouseEventArgs e)
        {
            TimeIntervalText.Visibility = Visibility.Hidden;
        }

        /// <summary>
        /// 编译代码
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void MenuCompile_Click(object sender, RoutedEventArgs e)
        {
            TipBoxText.Text = "正在编译…";
            (FindResource("ShowTipBox") as Storyboard).Begin();
            StringBuilder error_message = new StringBuilder( 128 );
            int isCompileFinished = etController.Initilialize_Machine(editorAdapter.GetAllText(), error_message);
            
            if ( isCompileFinished != 0 )
            {
                System.Windows.MessageBox.Show( error_message.ToString() );
            }
            else
            {
                TipBoxText.Text = "编译成功！";
                (FindResource("ShowTipBox") as Storyboard).Begin();
                //List<Item> li = etController.GetMemoryItems();
                //System.Windows.MessageBox.Show(etController.GetMemoryItems().ToString());
                //memoryWindow.MemoryDataGrid.ItemsSource = etController.GetMemoryItems();
            }
        }

    }
    
}
