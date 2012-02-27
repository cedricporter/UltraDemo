using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Reflection;
using System.Windows.Controls;
using System.Windows.Media;

/**
@date		:	2012/02/22
@filename	: 	AnimationPluginManager.cs
@author		:	屠文翔	
			 -     _     - 
			| | - (_) __| |
			| |/ /| |/ _  |
			|   < | |||_| |
			|_|\_\|_|\____|
@note		:	管理算法动画插件
**/

namespace UltraDemoInterface
{

    public class AnimationPluginManager
    {
        /// <summary>
        /// 从指定装载获取动画插件。
        /// 注意，这并不会冲掉之前装载的插件。若想要清空插件列表，请使用RemoveAllPlugins()。
        /// </summary>
        /// <param name="path"></param>
        public void LoadPlugins(String path)
        {
            String[] files;
            Assembly assembly;
            Type[] types;
            MethodInfo getName, getDescription, getWatchedList;
            Object anima, name, description, watchedList;

            // 由杨旭瑜发现的一个bug
            if (Directory.Exists(path) == false)
            {
                System.Windows.MessageBox.Show("你扫描插件的路径：" + path + "不存在，在该路径扫描插件失败。");
                return;
            }

            files = Directory.GetFiles(path);
            
            foreach (String file in files)
            {
                if (Path.GetExtension(file) == ".dll")
                {
                    assembly = Assembly.LoadFrom(file);
                    types = assembly.GetTypes();
                    foreach (Type t in types)
                    {
                        if (t.ToString() == "UltraDemoInterface.AnimationFactory")
                            continue;
                        MemberInfo[] token = t.GetMember("BeginRender");
                        if (token.Length > 0)
                        {
                            // 找到了入口
                            anima = Activator.CreateInstance(t, mainWindow.animationContainer);
                            getName = t.GetMethod("GetName");
                            getDescription = t.GetMethod("GetDescription");
                            getWatchedList = t.GetMethod("GetWatchedList");
                            name = getName.Invoke(anima, null);
                            description = getDescription.Invoke(anima, null);
                            watchedList = getWatchedList.Invoke(anima, null);

                            // 向SelectAnimationWindow添加信息
                            SelectAnimationWindow.AnimationInfo aniInfo = new SelectAnimationWindow.AnimationInfo();
                            aniInfo.description = description.ToString();
                            aniInfo.watchedList = (List<String>)watchedList;
                            mainWindow.selectAnimationWindow.animationInfoMap[name.ToString()] = aniInfo;
                            ListBoxItem item = new ListBoxItem();
                            item.Content = name.ToString();
                            item.Template = (ControlTemplate)App.Current.FindResource("ListBoxItemTemplate");
                            mainWindow.selectAnimationWindow.AnimationList.Items.Add(item);

                            //将动画类型装入动画列表
                            animationMap[name.ToString()] = t;
                            watchedListMap[t] = (List<String>)watchedList;
                        }
                    }
                }
            }
        }

        /// <summary>
        /// 移除所有动画
        /// </summary>
        public void RemoveAllPlugins()
        {

        }

        /// <summary>
        /// 供Composition.Target调用的动画回调函数
        /// </summary>
        /// <param name="sender">Threading.Dispatcher</param>
        /// <param name="e">RenderingEventArgs</param>
        public void RenderAnimationCallback(Object sender, EventArgs e)
        {
            if (isRendering == false)
                return;
            Dictionary<String, UInt32> map = mainWindow.etController.GetInterestingVariables();
            if (map == null)
                return;
            Object[] args = { sender, e, map };
            try
            {
                selectedAnimationBeginRender.Invoke( selectedAnimationInstance, args );
            }
            catch ( Exception ex )
            {
                StopRender();
                mainWindow.debugControler.Stop();
                System.Windows.MessageBox.Show( "404 动画模块死掉了" );
                System.Windows.MessageBox.Show( ex.ToString() );
            }
        }

        /// <summary>
        /// 开始运行动画
        /// 初始化动画实例，将标志位设为渲染
        /// </summary>
        public void BeginRender()
        {
            // 并没有选择任何动画
            if (selectedAnimationType == null)
            {
                // 此处应该弹出一个“友好”的对话框警告用户
                // 未实现
                System.Windows.MessageBox.Show("并没有选择任何动画");
                return;
            }

            // 实例化动画
            selectedAnimationInstance = Activator.CreateInstance(selectedAnimationType, mainWindow.animationContainer);
            // 此处应该判断一下该动画的检测列表是否与设置值一致
            // 未实现
            //MethodInfo getWatchedList = selectedAnimationType.GetMethod("GetWatchedList");
            //List<String> watchedList = (List<String>)getWatchedList.Invoke(selectedAnimationInstance, null);
            //判断 

            // 获取渲染方法
            selectedAnimationBeginRender = selectedAnimationType.GetMethod("BeginRender");
            if (selectedAnimationBeginRender == null)
            {
                System.Windows.MessageBox.Show("未找到BeginRender函数入口");
                return;
            }
            // 获取设置时间间隔方法
            selectedAnimationSetTimeInterval = selectedAnimationType.GetMethod("SetTimeInterval");
            if (selectedAnimationSetTimeInterval == null)
            {
                System.Windows.MessageBox.Show("未找到SetTimeInterval函数入口");
                return;
            }
            Object[] args = {timeInterval};
            selectedAnimationSetTimeInterval.Invoke(selectedAnimationInstance, args);

            // 清空画布
            mainWindow.animationContainer.Children.Clear();

            isRendering = true;
        }

        /// <summary>
        /// 停止运行动画
        /// 释放资源
        /// </summary>
        public void StopRender()
        {
            isRendering = false;
            selectedAnimationInstance = null;
            selectedAnimationBeginRender = null;
            selectedAnimationSetTimeInterval = null;
            // 清空画布
            mainWindow.animationContainer.Children.Clear();

            System.Windows.MessageBox.Show("动画渲染停止");
        }

        /// <summary>
        /// 选择要使用的动画
        /// </summary>
        /// <param name="animationName"></param>
        public void SelectAnimation(String animationName)
        {
            selectedAnimationType = animationMap[animationName];
            //System.Windows.MessageBox.Show(selectedAnimationType.ToString());
        }

        /// <summary>
        /// 设置动画的时间间隔
        /// </summary>
        /// <param name="interval"></param>
        public void SetTimeInterval(Double interval)
        {
            this.timeInterval = interval;
            if (isRendering == true)
            {
                Object[] args = { timeInterval };
                selectedAnimationSetTimeInterval.Invoke(selectedAnimationInstance, args);
            }
        }

        /// <summary>
        /// 返回当前动画的关注列表
        /// </summary>
        /// <returns></returns>
        public List<String> GetSelectedAnimationWatchedList()
        {
            if (selectedAnimationType == null)
                return null;
            return watchedListMap[selectedAnimationType];
        }
        

        /// <summary>
        /// 构造函数
        /// </summary>
        /// <param name="animationContainer"></param>
        /// <param name="selectAnimationWindow"></param>
        public AnimationPluginManager(MainWindow mainWindow)
        {
            this.mainWindow = mainWindow;
        }

        private Boolean isRendering = false;

        private Type selectedAnimationType = null;
        private Object selectedAnimationInstance = null;
        private MethodInfo selectedAnimationBeginRender = null;
        private MethodInfo selectedAnimationSetTimeInterval = null;
        private Double timeInterval = 500;
        public Dictionary<String, Type> animationMap = new Dictionary<String, Type>();
        public Dictionary<Type, List<String>> watchedListMap = new Dictionary<Type, List<String>>();

        private MainWindow mainWindow;

    }
}
