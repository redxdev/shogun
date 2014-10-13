using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sholan.Compiler.Nodes
{
    class ConstantBoolNode : AbstractCompileNode
    {
        public bool Value
        {
            get;
            set;
        }

        public ConstantBoolNode(int line, int col)
            : base(line, col)
        {
            this.Attributes
                .Has("value")
                .Has("constant");
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
            if(this.Value)
            {
                k.Emit(Opcode.PUSHT).SetDebug(Line, Column, DebugType.Value, "true");
            }
            else
            {
                k.Emit(Opcode.PUSHF).SetDebug(Line, Column, DebugType.Value, "false");
            }
        }
    }
}
