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
            //if (isRendering == false)
            //    return;


            //TimeSpan renderingTime = (e as RenderingEventArgs).RenderingTime;
            ////System.Windows.MessageBox.Show(sender.ToString());
            //System.Windows.MessageBox.Show(renderingTime.Milliseconds.ToString());
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
                return;
            }

            // 实例化动画
            selectedAnimationInstance = Activator.CreateInstance(selectedAnimationType, mainWindow.animationContainer);
            // 此处应该判断一下该动画的检测列表是否与设置值一致
            // 未实现
            //MethodInfo getWatchedList = selectedAnimationType.GetMethod("GetWatchedList");
            //List<String> watchedList = (List<String>)getWatchedList.Invoke(selectedAnimationInstance, null);
            //判断 

            selectedAnimationBeginRender = selectedAnimationType.GetMethod("BeginRender");
            if (selectedAnimationBeginRender == null)
            {
                // 此处应该弹出一个“友好”的对话框警告用户
                // 未实现
                return;
            }

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
            // 清空画布
            mainWindow.animationContainer.Children.Clear();
        }

        /// <summary>
        /// 选择要使用的动画
        /// </summary>
        /// <param name="animationName"></param>
        public void SelectAnimation(String animationName)
        {
            selectedAnimationType = animationMap[animationName];
            System.Windows.MessageBox.Show(selectedAnimationType.ToString());
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
        public Dictionary<String, Type> animationMap = new Dictionary<String, Type>();

        private MainWindow mainWindow;

    }
}
