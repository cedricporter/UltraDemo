using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

/**
@date		:	2012/02/22
@filename	: 	AnimationFactory.cs
@author		:	屠文翔	
			 -     _     - 
			| | - (_) __| |
			| |/ /| |/ _  |
			|   < | |||_| |
			|_|\_\|_|\____|
@note		:	动画工厂
**/

namespace UltraDemoInterface
{

    public abstract class AnimationFactory : Object
    {
        /// <summary>
        /// 测试用
        /// </summary>
        public void SayHello()
        {
            System.Windows.MessageBox.Show(this.ToString() + " say hello");
        }

        ///// <summary>
        ///// 子类通过重载该函数以实现单例
        ///// 注意：不实现此方法则无法加载dll
        ///// </summary>
        ///// <returns></returns>
        //public static AnimationFactory GetAnimationSingleton()
        //{
        //    System.Windows.MessageBox.Show("asdfafasdfadsf");
        //    return null;
        //}

        /// <summary>
        /// 子类通过重载此方法以实现动画
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        public abstract void BeginRender(Object sender, EventArgs e);

        /// <summary>
        /// 获取动画名称
        /// </summary>
        /// <returns>动画名称</returns>
        public virtual String GetName()
        {
            return name;
        }

        /// <summary>
        /// 获取动画描述
        /// </summary>
        /// <returns>动画描述</returns>
        public virtual String GetDescription()
        {
            return description;
        }

        /// <summary>
        /// 获取动画监视值列表
        /// </summary>
        /// <returns>监视值列表List<String></returns>
        public virtual List<String> GetWatchedList()
        {
            return watchedList;
        }

        ///// <summary>
        ///// 构造函数
        ///// </summary>
        //public AnimationFactory();

        /// <summary>
        /// 设置动画名称
        /// </summary>
        /// <param name="name">动画名称</param>
        protected virtual void SetName(String name)
        {
            this.name = name;
        }

        /// <summary>
        /// 设置动画描述
        /// </summary>
        /// <param name="description">动画描述</param>
        protected virtual void SetDescription(String description)
        {
            this.description = description;
        }

        /// <summary>
        /// 向监控值名称列表中加入新的监控名称
        /// </summary>
        /// <param name="varname">监控值名称</param>
        protected virtual void AddWatchedVarName(String varname)
        {
            watchedList.Add(varname);
        }

        protected String name;
        protected String description;
        protected List<String> watchedList;
        public static AnimationFactory instance = null;
    }
}
