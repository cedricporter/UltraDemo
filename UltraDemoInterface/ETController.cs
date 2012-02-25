using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace TestLoader
{
    class ETController
    {
        [DllImport("Compiler.dll", EntryPoint="get_dreg")]
        private static extern System.UInt64[] get_dreg(IntPtr ctrl);

        [DllImport("Compiler.dll", EntryPoint="get_reg")]
        private static extern System.UInt64[] get_reg(IntPtr ctrl);

        [DllImport("Compiler.dll", EntryPoint="create_controller")]
        private static extern IntPtr create_controller();

        [DllImport("Compiler.dll", EntryPoint="destroy_controller")]
        private static extern void destroy_controller(IntPtr ctrl);

        [DllImport("Compiler.dll", EntryPoint="initial_machine")]
        private static extern int initial_machine(IntPtr ctrl, string code);

        [DllImport("Compiler.dll", EntryPoint="step")]
        private static extern bool step(IntPtr ctrl);

        [DllImport("Compiler.dll", EntryPoint="get_current_line")]
        private static extern int get_current_line(IntPtr ctrl);

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

        public int Initilialize_Machine(string code)
        {
            return initial_machine( controller, code );
        }

        public int GetCurrentLine()
        {
            return get_current_line( controller );
        }



    }
}
