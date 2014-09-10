using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sholan.Compiler.Nodes
{
    public class RawAssemblyNode : AbstractCompileNode
    {
        public string Assembly
        {
            get;
            set;
        }

        public override void PrePass(Kernel k)
        {
        }

        public override void PreCompile(Kernel k)
        {
        }

        public override void Compile(Kernel k)
        {
            k.Emit(Opcode.NOOP).Comment = "begin user section";
            k.Emit(new Operation() { Raw = this.Assembly });
            k.Emit(Opcode.NOOP).Comment = "end user section";
        }
    }
}
