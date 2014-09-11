using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sholan.Compiler.Nodes
{
    public class NilValueNode : AbstractCompileNode
    {
        public NilValueNode()
            : base()
        {
            this.Attributes
                .Has("value")
                .Has("constant")
                .Has("nil");
        }

        public override void PrePass(Kernel k)
        {
        }

        public override void PreCompile(Kernel k)
        {
        }

        public override void Compile(Kernel k)
        {
            k.Emit(Opcode.PUSHNIL);
        }
    }
}
