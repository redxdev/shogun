using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sholan.Compiler.Nodes
{
    public class HaltNode : AbstractCompileNode
    {
        public HaltNode(int line, int col)
            : base(line, col)
        {
            this.Attributes
                .Has("halt")
                .Has("scope-end")
                .Has("forceful-end");
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
            k.Emit(Opcode.HALT).SetDebug(Line, Column, DebugType.Raw, "halt");
        }
    }
}
