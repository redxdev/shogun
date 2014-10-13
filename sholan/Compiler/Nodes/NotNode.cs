using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sholan.Compiler.Nodes
{
    public class NotNode : AbstractCompileNode
    {
        public ICompileNode Value
        {
            get;
            set;
        }

        public NotNode(int line, int col)
            : base(line, col)
        {
            this.Attributes
                .Has("value");
        }

        public override void PrePass(Kernel k)
        {
            base.PrePass(k);

            this.Value.Attributes
                .Check("value");

            this.Value.PrePass(k);
        }

        public override void PreCompile(Kernel k)
        {
            this.Value.PreCompile(k);
        }

        public override void Compile(Kernel k)
        {
            this.Value.Compile(k);

            k.Emit(Opcode.NOT);
        }
    }
}
