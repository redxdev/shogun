using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using sholan.Compiler.Nodes;

namespace sholan.Compiler
{
    [Serializable]
    public class CompileException : Exception
    {
        public CompileException()
            : base()
        {
        }

        public CompileException(string message)
            : base(message)
        {
        }

        public CompileException(string message, Exception inner)
            : base(message, inner)
        {
        }
    }
}
