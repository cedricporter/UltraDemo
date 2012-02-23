using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;

namespace UltraDemoInterface
{
    public class ArrayAnimation : AnimationFactory
    {
        public ArrayAnimation()
        {
            name = "ArrayAnimation";
            description = "如果希望动画可以监控您的代码，请以关键字命名您希望被监控关键字。以watchedArray命名数组指针，以watchedSize命名数组长度";
            //System.Windows.MessageBox.Show("ArrayAnimation Created");
        }

        public override void BeginRender(Object sender, EventArgs e)
        {
            throw new Exception("The method or operation is not implemented.");
        }
    }
}
