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
    public unsafe class ArrayAnimation : AnimationFactory
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
            //fuckr = new Rectangle();
            //Color bg = new Color();
            //bg.A = 255;
            //fuckr.Width = 100;
            //fuckr.Height = 100;
            //fuckr.Fill = new SolidColorBrush(bg);
            //animationContainer.Children.Add(fuckr);
            
        }

        public void DrawArray(int* array, int size)
        {
            animationContainer.Children.Clear();

            int j = 0;
            for (int i = 0; i < size; i++)
            {
                Rectangle rect = new Rectangle();

                TextBox text = new TextBox();
                text.Text = array[i].ToString();
                text.Height = 30;

                rect.Height = array[i] /  50;
                rect.Width = 30;
                rect.Fill = new SolidColorBrush(Colors.Red);

                TranslateTransform ts = new TranslateTransform((-200 + 50 * i) % 300, j * 30);

                rect.RenderTransform = ts;
                text.RenderTransform = ts;

                //animationContainer.Children.Add(rect); 
                animationContainer.Children.Add(text);

                if ((-200 + 50 * i) % 300 == 0)
                    j++;
            } 
        }

        public override void BeginRender(Object sender, EventArgs e, Dictionary<String, UInt32> map)
        {
            int* array = (int*)*(int*)map["watchedArray"];
            int size = *(int*)map["watchedSize"];

            if (array != (int*)0)
            {
                DrawArray(array, size);
            }

        }
    }
}
