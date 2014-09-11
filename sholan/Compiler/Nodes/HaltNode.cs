using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sholan.Compiler.Nodes
{
    public class HaltNode : AbstractCompileNode
    {
        public HaltNode()
            : base()
        {
            this.Attributes
                .Has("halt")
                .Has("scope-end")
                .Has("forceful-end");
        }

        public override void PrePass(Kernel k)
        {
        }

        public override void PreCompile(Kernel k)
        {
        }

        public override void Compile(Kernel k)
        {
            k.Emit(Opcode.HALT);
        }
    }
}
