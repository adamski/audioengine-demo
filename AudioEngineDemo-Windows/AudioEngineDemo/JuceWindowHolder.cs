﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Interop;

namespace AudioEngineDemo
{
    class JuceWindowHolder : HwndHost
    {
        public JuceWindowHolder()
        {
        }

        protected override HandleRef BuildWindowCore(HandleRef hwndParent)
        {
            // TODO
            throw new NotImplementedException();
        }

        protected override void DestroyWindowCore(HandleRef hwnd)
        {
            // TODO
        }
    }
}
