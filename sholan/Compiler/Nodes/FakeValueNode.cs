using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sholan.Compiler.Nodes
{
    public class FakeValueNode : AbstractCompileNode
    {
        public string VariableName
        {
            get;
            set;
        }

        public FakeValueNode(int line, int col)
            : base(line, col)
        {
            this.Attributes
                .Has("value");
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
        }
    }
}
