using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Media;

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
            bool isRunning = mainWindow.etController.Step();
            currLineNumber = mainWindow.etController.GetCurrentLine();
            // 通知编辑窗口更新行号
            // ...
            mainWindow.editorAdapter.ShowLine(true, currLineNumber);

            if (isRunning == false)
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
            currLineNumber = 0;

            state = DebugControlerState.DCS_RUN;
        }

        /// <summary>
        /// 单步
        /// </summary>
        public void Step()
        {
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

    }
}
