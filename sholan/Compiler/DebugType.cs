using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sholan.Compiler
{
    public enum DebugType
    {
        None,
        Raw, // assembly
        Retrieve,
        Call,
        ECall,
        LCall,
        Branch,
        ForLoop,
        WhileLoop,
        DoLoop,
        Break,
        Return,
        Value,
        Debug,
        Define,
        Set,
        PushMem,
        PopMem,
        Function,
        Operation,
        Import
    }
}
