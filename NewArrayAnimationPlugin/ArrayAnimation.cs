using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Shapes;
using System.Windows.Media;

namespace UltraDemoInterface
{
    public class ArrayAnimation : AnimationFactory
    {
        Rectangle fuckr;

        public ArrayAnimation( Grid animationContainer )
        {
            this.animationContainer = animationContainer;

            name = "ArrayAnimation";
            description = "如果希望动画可以监控您的代码，请以关键字命名您希望被监控关键字。以watchedArray命名数组指针，以watchedSize命名数组长度";
            watchedList = new List<String>();
            watchedList.Add("watchedArray");
            watchedList.Add("watchedSize");
            //System.Windows.MessageBox.Show("ArrayAnimation Created");

            //testing
            fuckr = new Rectangle();
            Color bg = new Color();
            bg.A = 255;
            fuckr.Width = 100;
            fuckr.Height = 100;
            fuckr.Fill = new SolidColorBrush(bg);
            animationContainer.Children.Add(fuckr);
            
        }

        public override void BeginRender(Object sender, EventArgs e)
        {
            //System.Windows.MessageBox.Show("ArrayAnimation Rendering");
            fuckr.Width++;
        }
    }
}
