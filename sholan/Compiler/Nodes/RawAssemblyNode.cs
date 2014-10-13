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

        public RawAssemblyNode(int line, int col)
            : base(line, col)
        {
            this.Attributes
                .Has("assembly");
        }

        public override void PrePass(Kernel k)
        {
            base.PrePass(k);
        }

        public override void PreCompile(Kernel k)
        {
            base.PrePass(k);
        }

        public override void Compile(Kernel k)
        {
            k.Emit(Opcode.NOOP).SetDebug(File, Line, Column, DebugType.Raw, "user");
            k.Emit(new Operation() { Raw = this.Assembly });
            k.Emit(Opcode.NOOP).Comment = "end user section";
        }
    }
}
