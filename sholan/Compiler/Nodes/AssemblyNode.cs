using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sholan.Compiler.Nodes
{
    public class AssemblyNode : AbstractCompileNode
    {
        public Operation Operation
        {
            get;
            set;
        }

        public AssemblyNode(int line, int col)
            : base(line, col)
        {
        }

        public override void PrePass(Kernel k)
        {
            base.PrePass(k);
        }

        public override void PreCompile(Kernel k)
        {
        }

        public override void Compile(Kernel k)
        {
            k.Emit(this.Operation).SetDebug(Line, Column, DebugType.Raw, "");
        }
    }
}
