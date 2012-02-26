using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace UltraDemoInterface
{
    public class Item
    {
        public string name { get; set; }
        public string val { get; set; }

        public Item(string n, string v)
        {
            name = n;
            val = v;
        }
    }

    public class ETController
    {
        [DllImport("Compiler.dll", EntryPoint="create_controller", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr create_controller();

        [DllImport("Compiler.dll", EntryPoint="get_first_item", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr get_first_item(IntPtr ctrl);

        [DllImport("Compiler.dll", EntryPoint="get_next_item", CallingConvention= CallingConvention.Cdecl)]
        private static extern IntPtr get_next_item(IntPtr ctrl);

        [DllImport("Compiler.dll", EntryPoint="destroy_controller", CallingConvention= CallingConvention.Cdecl)]
        private static extern void destroy_controller(IntPtr ctrl);

        [DllImport("Compiler.dll", EntryPoint="initial_machine", CallingConvention= CallingConvention.Cdecl)]
        private static extern int initial_machine(IntPtr ctrl, string code, StringBuilder error_message);

        [DllImport("Compiler.dll", EntryPoint="step", CallingConvention= CallingConvention.Cdecl)]
        private static extern int step(IntPtr ctrl);

        [DllImport("Compiler.dll", EntryPoint="get_current_line", CallingConvention= CallingConvention.Cdecl)]
        private static extern int get_current_line(IntPtr ctrl);

        [StructLayout( LayoutKind.Sequential )]
        public struct MemItem
        {
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
            public string name;
            public UInt32 val;
        }

        private List<string> insteretingVariableNameList;

        IntPtr controller;
        public ETController()
        {
            controller = create_controller();
        }

        ~ETController()
        {
            destroy_controller( controller );
        }

        public int Step()
        {
            int ret = step( controller );
            //if ( ret == 0 )
            //    System.Windows.MessageBox.Show( "Done." );
            return ret;
        }

        public int Initilialize_Machine(string code, StringBuilder error_message, List<string> interestList)
        {
            insteretingVariableNameList = interestList;
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

            while (ret != IntPtr.Zero)
            {
                MemItem memitem = (MemItem)Marshal.PtrToStructure(ret, typeof(MemItem));
                Item item = new Item(memitem.name, memitem.val.ToString());
                list.Add(item);
                ret = get_next_item(controller);
            }
            
            return list; 
        }

        public List<MemItem> GetInterestingVariables()
        {
            if ( insteretingVariableNameList == null )
                return null;

            List<MemItem> list = new List<MemItem>();

            IntPtr ret = get_first_item(controller);

            while (ret != IntPtr.Zero)
            {
                MemItem memitem = (MemItem)Marshal.PtrToStructure(ret, typeof(MemItem));
                string varname = memitem.name;
                foreach (string n in insteretingVariableNameList)
                {
                    if ( varname == n )
                    {
                        list.Add( memitem );
                        break;
                    }
                }
                list.Add(memitem);
                ret = get_next_item(controller);
            }

            if ( list.Count != insteretingVariableNameList.Count )
                list = null;
            return list;
        }


    }
}
