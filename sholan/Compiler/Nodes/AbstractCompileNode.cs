using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sholan.Compiler.Nodes
{
    public abstract class AbstractCompileNode : ICompileNode
    {
        public int Line
        {
            get;
            set;
        }

        public int Column
        {
            get;
            set;
        }

        public string File
        {
            get;
            set;
        }
    }
}
