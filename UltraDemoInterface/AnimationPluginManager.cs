using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Reflection;

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

    class AnimationPluginManager
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
                            //System.Windows.MessageBox.Show(t.ToString());
                            //MethodInfo m = t.GetMethod("SayHello");
                            //obj = Activator.CreateInstance(t);
                            anima = Activator.CreateInstance(t);
                            getName = t.GetMethod("GetName");
                            getDescription = t.GetMethod("GetDescription");
                            getWatchedList = t.GetMethod("GetWatchedList");
                            name = getName.Invoke(anima, null);
                            System.Windows.MessageBox.Show(name.ToString());
                            description = getDescription.Invoke(anima, null);
                            System.Windows.MessageBox.Show(description.ToString());
                            //MethodInfo getName = t.GetMethod("GeName");
                            //if (m != null)
                            //{
                            //    obj = Activator.CreateInstance(t);
                            //    //getName.Invoke(obj, null);
                            //    //Object name = getName.Invoke(obj, null);
                            //    //System.Windows.MessageBox.Show(name.ToString());
                            //}



                            //(obj as AnimationFactory).SayHello();
                            //anima = (obj as AnimationFactory);
                            //anima.SayHello();
                        }
                    }
                    //mi = type.GetMethod("GetAnimationSingleton");
                        //System.Windows.MessageBox.Show(mi.IsStatic.ToString());
                    //mi.Invoke(null, null);
                    //Type[]ts = assembly.GetTypes();
                    //foreach (Type t in ts)
                    //{
                    //    Object obj =System.Activator.CreateInstance(t);
                    //    System.Windows.MessageBox.Show(obj.GetType().ToString());
                    //}
                }
            }
            //System.Windows.MessageBox.Show(files.Length.ToString());
        }

        public void RemoveAllPlugins()
        {

        }
    }
}
