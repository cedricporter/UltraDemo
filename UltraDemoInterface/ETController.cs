using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace UltraDemoInterface
{
    public class Item
    {
        public string name;
        public string val;

        public Item(string n, string v)
        {
            name = n;
            val = v;
        }
    }

    public class ETController
    {
        [DllImport("Compiler.dll", EntryPoint="get_dreg")]
        private static extern System.UInt64[] get_dreg(IntPtr ctrl);

        [DllImport("Compiler.dll", EntryPoint="get_reg")]
        private static extern System.UInt64[] get_reg(IntPtr ctrl);

        [DllImport("Compiler.dll", EntryPoint="create_controller")]
        private static extern IntPtr create_controller();

        [DllImport("Compiler.dll", EntryPoint="get_first_item")]
        private static extern IntPtr get_first_item(IntPtr ctrl);

        [DllImport("Compiler.dll", EntryPoint="get_next_item")]
        private static extern IntPtr get_next_item(IntPtr ctrl);

        [DllImport("Compiler.dll", EntryPoint="destroy_controller")]
        private static extern void destroy_controller(IntPtr ctrl);

        [DllImport("Compiler.dll", EntryPoint="initial_machine")]
        private static extern int initial_machine(IntPtr ctrl, string code, StringBuilder error_message);

        [DllImport("Compiler.dll", EntryPoint="step")]
        private static extern bool step(IntPtr ctrl);

        [DllImport("Compiler.dll", EntryPoint="get_current_line")]
        private static extern int get_current_line(IntPtr ctrl);

        [StructLayout( LayoutKind.Sequential )]
        public struct MemItem
        {
            public string name;
            public UInt32 val;
        }

        IntPtr controller;
        public ETController()
        {
            controller = create_controller();
        }

        ~ETController()
        {
            destroy_controller( controller );
        }

        public bool Step()
        {
            return step( controller );
        }

        public int Initilialize_Machine(string code, StringBuilder error_message)
        {
            return initial_machine( controller, code, error_message );
        }

        public int GetCurrentLine()
        {
            return get_current_line( controller );
        }

        public List<Item> GetMemoryItems()
        {
            List<Item> list = new List<Item>();

            IntPtr ret = get_first_item(controller);

            while (ret != null)
            {
                MemItem memitem = (MemItem)Marshal.PtrToStructure(ret, typeof(MemItem));
                Item item = new Item(memitem.name, memitem.val.ToString());
                list.Add(item);
                ret = get_next_item(controller);
            }
            
            return list; 
        }



    }
}
