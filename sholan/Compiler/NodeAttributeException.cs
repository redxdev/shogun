using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sholan.Compiler
{
    [Serializable]
    public class NodeAttributeException : CompileException
    {
        public NodeAttributeException(string message)
            : base(message)
        {
        }

        public NodeAttributeException(string message, Exception inner)
            : base(message, inner)
        {
        }
    }
}
