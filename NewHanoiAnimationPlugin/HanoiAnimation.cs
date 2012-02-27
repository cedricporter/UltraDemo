using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Shapes;
using System.Windows.Media;
using System.Windows.Media.Animation;

namespace UltraDemoInterface
{
    public unsafe class HanoiAnimation : AnimationFactory
    {
        // 汉诺塔的三根柱子
        Stack<Rectangle> pole1 = new Stack<Rectangle>();
        Stack<Rectangle> pole2 = new Stack<Rectangle>();
        Stack<Rectangle> pole3 = new Stack<Rectangle>();
        Dictionary<String, Stack<Rectangle>> poleMap = new Dictionary<String, Stack<Rectangle>>();

        public HanoiAnimation(Grid animationContainer)
        {
            this.animationContainer = animationContainer;

            name = "HanoiAnimation";
            description = "如果希望动画可以监控您的代码，请以关键字命名您希望被监控关键字。以watchedn命名汉诺塔数目，以watchedStart命名移出的塔编号，以watchedGoal命名移入的塔编号";
            watchedList.Add("watchedn");
            watchedList.Add("watchedStart");
            watchedList.Add("watchedGoal");

            poleMap["1"] = pole1;
            poleMap["2"] = pole2;
            poleMap["3"] = pole3;
        }

        // 监测关键变量
        int* size = null;
        int* from = null;
        int* to = null;
        int oldSize, oldFrom, oldTo;

        Boolean isInitialized = false;
        int baseItemWidth = 120;
        int baseItemHeight = 20;
        int poleInterval = 200;
        int poleHeight = 200;
        int poleWidth = 10;

        /// <summary>
        /// 初始化
        /// </summary>
        void Initialize()
        {
            SolidColorBrush brb = new SolidColorBrush(Color.FromRgb(0, 0, 0));
            SolidColorBrush brw = new SolidColorBrush(Color.FromRgb(255, 255, 255));
            // 初始化柱子
            TranslateTransform tst1 = new TranslateTransform(-poleInterval, 0);
            TranslateTransform tst2 = new TranslateTransform(poleInterval, 0);
            Rectangle pole1Rect = new Rectangle();
            Rectangle pole2Rect = new Rectangle();
            Rectangle pole3Rect = new Rectangle();
            pole1Rect.Height = pole2Rect.Height = pole3Rect.Height = poleHeight;
            pole1Rect.Width = pole2Rect.Width = pole3Rect.Width = poleWidth;
            pole1Rect.Fill = pole2Rect.Fill = pole3Rect.Fill = brw;
            pole1Rect.Stroke = pole2Rect.Stroke = pole3Rect.Stroke = brb;
            pole1Rect.RenderTransform = tst1;
            pole3Rect.RenderTransform = tst2;
            animationContainer.Children.Add(pole1Rect);
            animationContainer.Children.Add(pole2Rect);
            animationContainer.Children.Add(pole3Rect);

            // 初始化盘面
            if (oldSize == 0)
            {
                System.Windows.MessageBox.Show("盘面数量为零");
                return;
            }
            Double offset = baseItemWidth / oldSize;
            for (int i = 0; i < oldSize; i++)
            {
                Rectangle rect = new Rectangle();

                rect.Fill = brw;
                rect.Stroke = brb;
                rect.Height = baseItemHeight;
                rect.Width = baseItemWidth - i * offset;
                TranslateTransform ts = new TranslateTransform(-poleInterval, (poleHeight-baseItemHeight)/2 + i * -baseItemHeight);
                rect.RenderTransform = ts;
                pole1.Push(rect);
                animationContainer.Children.Add(rect);
            }
        }

        void MoveItem(Stack<Rectangle> fromPole, Stack<Rectangle> toPole)
        {
            // 逻辑
            Rectangle item = fromPole.Pop();
            int desPoleItemNum = toPole.Count;
            toPole.Push(item);

            // 动画
            Double offsetX, offsetY;
            if (toPole == pole1) offsetX = -poleInterval;
            else if (toPole == pole2) offsetX = 0;
            else offsetX = poleInterval;
            offsetY = (poleHeight - baseItemHeight) / 2 + desPoleItemNum * -baseItemHeight;
            TranslateTransform ts = new TranslateTransform(offsetX, offsetY);
            item.RenderTransform = ts;

            //Double offsetX, offsetY;
            //offsetX = item.RenderTransform.Value.OffsetX;
            //offsetY = item.RenderTransform.Value.OffsetY;
            //TranslateTransform ts = new TranslateTransform(offsetX, offsetY);
            //DoubleAnimationUsingKeyFrames ani = new DoubleAnimationUsingKeyFrames();
            //ani.KeyFrames.Add(new LinearDoubleKeyFrame(-500, TimeSpan.FromMilliseconds(400)));
            //ani.KeyFrames.Add(new LinearDoubleKeyFrame(offsetY, TimeSpan.FromMilliseconds(800)));
        }


        public override void BeginRender(Object sender, EventArgs e, Dictionary<String, UInt32> map)
        {
            if (map == null)
                return;

            size = (int*)map["watchedn"];
            from = (int*)map["watchedStart"];
            to = (int*)map["watchedGoal"];

            if (size != (int*)0)
            {
                if (isInitialized == false)
                {
                    isInitialized = true;
                    oldSize = *size;
                    oldFrom = *from;
                    oldTo = *to;
                    Initialize();
                    
                    //MoveItem(pole1, pole3);
                    //MoveItem(pole1, pole2);
                    //MoveItem(pole3, pole2);
                    MoveItem(poleMap[oldFrom.ToString()], poleMap[oldTo.ToString()]); 
                }
                if ((*from != oldFrom || *to != oldTo) && poleMap[(*from).ToString()].Count > 0 && poleMap[(*to).ToString()].Count < oldSize)
                {
                    oldFrom = *from;
                    oldTo = *to;  
                    MoveItem(poleMap[oldFrom.ToString()], poleMap[oldTo.ToString()]); 
                }
            }          
        }

        
    }
}
