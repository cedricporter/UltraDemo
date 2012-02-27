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

        [DllImport("Compiler.dll", EntryPoint="get_output", CallingConvention= CallingConvention.Cdecl)]
        private static extern int get_output(IntPtr ctrl, StringBuilder outputString);

        [StructLayout( LayoutKind.Sequential )]
        public struct MemItem
        {
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
            public string name;
            public UInt32 val;
        }

        private List<string> insteretingVariableNameList;
        private StringBuilder outString = new StringBuilder( 1024 * 32 );

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

        public void SetInterestringList(List<string> list)
        {
            insteretingVariableNameList = list;
        }

        public int Initilialize_Machine(string code, StringBuilder error_message)
        {
            return initial_machine( controller, code, error_message );
        }

        public int GetCurrentLine()
        {
            return get_current_line( controller );
        }

        public string GetOutput()
        {
            get_output(controller, outString);
            return outString.ToString();
        }

        public List<Item> GetMemoryItems()
        {
            List<Item> list = new List<Item>();

            IntPtr ret = get_first_item(controller);

            while (ret != IntPtr.Zero)
            {
                // val默认为变量的内存地址，所以我们要去那个地址去取值。
                MemItem memitem = (MemItem)Marshal.PtrToStructure(ret, typeof(MemItem));
                UInt32 val;
                unsafe
                {
                    val = *(UInt32*)memitem.val;
                }
                Item item = new Item(memitem.name, val.ToString());
                list.Add(item);
                ret = get_next_item(controller);
            }
            
            return list; 
        }

        public Dictionary<String, UInt32> GetInterestingVariables()
        {           
            if ( insteretingVariableNameList == null )
                return null;

            Dictionary<String, UInt32> map = new Dictionary<String, UInt32>();
            
            IntPtr ret = get_first_item(controller);

            while (ret != IntPtr.Zero)
            {
                MemItem memitem = (MemItem)Marshal.PtrToStructure(ret, typeof(MemItem));
                string varname = memitem.name;
                foreach (string n in insteretingVariableNameList)
                {
                    if ( varname == n )
                    {
                        map[memitem.name] = memitem.val;
                        //map.Add(new KeyValuePair<string, UInt32>(memitem.name, memitem.val));
                        break;
                    }
                }
  
                ret = get_next_item(controller);
            }

            if (map.Count != insteretingVariableNameList.Count)
                map = null;
            return map;
        }


    }
}
