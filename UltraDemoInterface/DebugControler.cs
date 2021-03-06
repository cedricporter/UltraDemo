﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Controls;
using System.Windows.Documents;

/**
@date		:	2012/02/24
@filename	: 	DebugControler.cs
@author		:	屠文翔	
			 -     _     - 
			| | - (_) __| |
			| |/ /| |/ _  |
			|   < | |||_| |
			|_|\_\|_|\____|
@note		:	控制代码运行的速度
**/

namespace UltraDemoInterface
{
    public class DebugControler
    {
        /// <summary>
        /// 调试器运行状态
        /// </summary>
        public enum DebugControlerState
        {
            DCS_RUN, DCS_STEP, DCS_STOP, DCS_PAUSE, DCS_IDLE
        }

        /// <summary>
        /// 控制器的主循环
        /// </summary>
        /// <param name="sender">Threading.Dispatcher</param>
        /// <param name="e">RenderingEventArgs</param>
        public void DebugControlerCallback(Object sender, EventArgs e)
        {
            currTime = (e as RenderingEventArgs).RenderingTime.TotalMilliseconds;

            switch (state)
            {
                case DebugControlerState.DCS_RUN:
                    {
                        if (currTime - lastTime < timeInterval)
                            break;
                        lastTime = currTime;
                        ExecuteOneLine();
                    }
                    break;

                case DebugControlerState.DCS_STEP:
                    {
                        ExecuteOneLine();
                        state = DebugControlerState.DCS_STOP;
                    }
                    break;

                case DebugControlerState.DCS_STOP:
                    //System.Windows.MessageBox.Show((e as RenderingEventArgs).RenderingTime.TotalMilliseconds.ToString());
                    // 啥也不做
                    break;

                case DebugControlerState.DCS_PAUSE:
                    // 啥也不做
                    break;
            }
        }

        /// <summary>
        /// 解释执行一行源代码
        /// </summary>
        public void ExecuteOneLine()
        {
            // 调用Dll的单步
            int isRunning = mainWindow.etController.Step();
            // 更新内存窗口
            if (mainWindow.memoryWindow.IsVisible == true)
            {
                mainWindow.memoryWindow.MemoryDataGrid.ItemsSource = mainWindow.etController.GetMemoryItems();
            } 
            // 高亮编辑器的相应行
            if (mainWindow.DemoViewButton.IsChecked != true)
            {
                currLineNumber = mainWindow.etController.GetCurrentLine();
                // 定义高亮行的颜色
                Color c = new Color();
                c.R = 255;
                c.G = 214;
                c.B = 84;
                c.A = 255;
                mainWindow.editorAdapter.ShowLine(true, currLineNumber, c);
            }
            // 更新断点列表并提供断点服务
            breakPointList = mainWindow.editorAdapter.getBreakPointList();
            foreach (int bp in breakPointList)
            {
                if (bp == currLineNumber)
                {
                    state = DebugControlerState.DCS_STOP;
                    mainWindow.StepButton.IsEnabled = true;
                    mainWindow.RunButton.IsChecked = false;
                    mainWindow.TipBoxText.Text = "到达断点！";
                    (mainWindow.FindResource("ShowTipBox") as Storyboard).Begin();
                    break;
                }
            }
            // 更新输出窗口信息
            if (mainWindow.outputWindow.IsVisible)
            {
                Run myRun = new Run(mainWindow.etController.GetOutput());
                Paragraph myParagraph = new Paragraph();
                myParagraph.Inlines.Add(myRun);
                mainWindow.outputWindow.OutputBox.Document.Blocks.Clear();
                mainWindow.outputWindow.OutputBox.Document.Blocks.Add(myParagraph);
            }

            if (isRunning == 0)
                Stop();
        }

        /// <summary>
        /// 通过设置运行时间间隔来控制运行的速度
        /// </summary>
        public void SetTimeInterval(Double interval)
        {
            this.timeInterval = interval;
        }

        /// <summary>
        /// 运行
        /// </summary>
        public void Run()
        {
            // 初始化
            lastTime = currTime;
            state = DebugControlerState.DCS_RUN;
        }

        /// <summary>
        /// 单步
        /// </summary>
        public void Step()
        {
            // 初始化
            lastTime = currTime;
            state = DebugControlerState.DCS_STEP;
        }

        /// <summary>
        /// 暂停
        /// </summary>
        public void Pause()
        {
            state = DebugControlerState.DCS_PAUSE;
        }

        /// <summary>
        /// 继续
        /// </summary>
        public void Resume()
        {
            state = DebugControlerState.DCS_RUN;
        }

        /// <summary>
        /// 停止
        /// </summary>
        public void Stop()
        {
            state = DebugControlerState.DCS_STOP;
            // 释放资源
            currLineNumber = 0;
            mainWindow.memoryWindow.MemoryDataGrid.ItemsSource = null;
            breakPointList = null;

            System.Windows.MessageBox.Show("解释器停止");
            // 停止渲染
            mainWindow.animationPluginManager.StopRender();
            //System.Windows.MessageBox.Show( mainWindow.etController.GetOutput() );
        }

        public DebugControler(MainWindow mainWindow)
        {
            this.mainWindow = mainWindow;
        }

        MainWindow mainWindow;

        DebugControlerState state = DebugControlerState.DCS_STOP;
        Double timeInterval = 500;
        Double currTime = Double.NaN;
        Double lastTime = Double.NaN;
        int currLineNumber = 0;
        List<int> breakPointList = null;

    }
}
