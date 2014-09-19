using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sholan.Compiler.Nodes
{
    public class CompilerBreakNode : AbstractCompileNode
    {
        public CompilerBreakNode()
            : base()
        {
            this.Attributes
                .Has("debug")
                .Has("compiler");
        }

        public override void PrePass(Kernel k)
        {
        }

        public override void PreCompile(Kernel k)
        {
        }

        public override void Compile(Kernel k)
        {
            Debugger.Break();
        }
    }
}
