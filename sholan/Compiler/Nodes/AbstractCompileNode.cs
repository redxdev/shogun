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

        public bool UseReturn
        {
            get;
            set;
        }

        public NodeAttributes Attributes
        {
            get;
            protected set;
        }

        public AbstractCompileNode(int line, int col)
        {
            this.Line = line;
            this.Column = col;
            this.Attributes = new NodeAttributes();
        }

        public virtual void PrePass(Kernel k)
        {
            this.File = k.FileStack.Peek();
        }

        public abstract void PreCompile(Kernel k);

        public abstract void Compile(Kernel k);
    }
}
