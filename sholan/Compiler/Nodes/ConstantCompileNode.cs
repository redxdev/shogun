using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sholan.Compiler.Nodes
{
    public class ConstantCompileNode : AbstractCompileNode
    {
        public string Value
        {
            get;
            set;
        }

        public ConstantCompileNode()
            : base()
        {
            this.Value = null;

            this.Attributes
                .Has("value")
                .Has("constant");
        }

        public override void PrePass(Kernel k)
        {
        }

        public override void PreCompile(Kernel k)
        {
        }

        public override void Compile(Kernel k)
        {
            if(this.Value == null)
            {
                k.Emit(Opcode.PUSHNIL);
            }
            else
            {
                k.EmitPush(this.Value);
            }
        }
    }
}
