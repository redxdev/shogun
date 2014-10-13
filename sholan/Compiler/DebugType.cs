using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sholan.Compiler
{
    public enum DebugType
    {
        NONE,
        RAW, // assembly
        RETRIEVE,
        CALL,
        ECALL,
        BRANCH,
        FOR_LOOP,
        WHILE_LOOP,
        DO_LOOP
    }
}
