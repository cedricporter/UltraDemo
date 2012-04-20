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
            watchedList.Add("cnt");
            watchedList.Add("b");
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

        public void DrawArray(int* array, int size, int y, string name)
        { 
            TranslateTransform infots = new TranslateTransform((-260) % 300, y);
            TextBlock infoText = new TextBlock();
            infoText.RenderTransform = infots;
            infoText.Text = name;
            infoText.Height = 30;
            infoText.Width = 50;
            animationContainer.Children.Add(infoText); 

            for (int i = 0; i < size; i++)
            {
                Rectangle rect = new Rectangle();

                TextBlock text = new TextBlock();
                text.Text = array[i].ToString();
                text.Height = 30;
                text.Width = 50;

                rect.Height = array[i] ;
                rect.Width = 30;
                rect.Fill = new SolidColorBrush(Colors.LightBlue);


                TranslateTransform textts = new TranslateTransform((-200 + 50 * i) % 300, y);
                TranslateTransform backts = new TranslateTransform((-215 + 50 * i) % 300, y);

                text.RenderTransform = textts;
                rect.RenderTransform = backts;

                animationContainer.Children.Add(rect); 
                animationContainer.Children.Add(text);
            } 
        }

        public override void BeginRender(Object sender, EventArgs e, Dictionary<String, UInt32> map)
        {
            int* array = (int*)*(int*)map["watchedArray"];
            int* cnt = (int*)*(int*)map["cnt"];
            int* b = (int*)*(int*)map["b"];
            int size = *(int*)map["watchedSize"];

            if (array != (int*)0)
            {
                animationContainer.Children.Clear();
                DrawArray(array, size, 100, "array");
                DrawArray(cnt, size, 0, "cnt");
                DrawArray(b, size, -100, "b");
            }

        }
    }
}
