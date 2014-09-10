using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sholan.Compiler.Nodes.Arguments
{
    public class ConstantArgument : IArgument
    {
        public string Value
        {
            get;
            set;
        }

        public void PushValue(Kernel k)
        {
            if (this.Value == null)
                k.Emit(Opcode.PUSHNIL).Comment = "constant argument";
            else
                k.EmitPush(this.Value).Comment = "constant argument";
        }
    }
}
