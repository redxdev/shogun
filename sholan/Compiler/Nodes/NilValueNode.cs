using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sholan.Compiler.Nodes
{
    public class NilValueNode : AbstractCompileNode
    {
        public NilValueNode(int line, int col)
            : base(line, col)
        {
            this.Attributes
                .Has("value")
                .Has("constant")
                .Has("nil");
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
            k.Emit(Opcode.PUSHNIL).SetDebug(File, Line, Column, DebugType.Value, "nil");
        }
    }
}
