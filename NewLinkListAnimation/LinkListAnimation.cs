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
    public unsafe class LinkListAnimation : AnimationFactory
    {

        struct Node
        {
            int val;
            Node* next;
        };

        Node* head;

        public LinkListAnimation(Grid animationContainer)
        {
            this.animationContainer = animationContainer;

            name = "LinkedListAnimation";
            description = "如果希望动画可以监控您的代码，请以关键字命名您希望被监控关键字。以watchedNode命名结构体指针。结构体定义为struct Node\n{\nint val;\nNode* next;\n};";
            //watchedList.Add("watchedpNode");
            watchedList.Add("watchedHead");
        }

        public override void BeginRender(Object sender, EventArgs e, Dictionary<String, UInt32> map)
        {
            head = *(Node**)map["watchedHead"];
        }

    }
}
