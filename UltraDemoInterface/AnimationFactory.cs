using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;

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

    public abstract class AnimationFactory
    {
        /// <summary>
        /// 测试用
        /// </summary>
        public void SayHello()
        {
            System.Windows.MessageBox.Show(this.ToString() + " say hello");
        }

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

        protected String name;
        protected String description;
        protected List<String> watchedList;
        public Grid animationContainer;
        //{
        //    get { return animationContainer; }
        //    set { animationContainer = (Grid)value; }
        //}
        //protected static AnimationFactory instance = null;
    }
}
