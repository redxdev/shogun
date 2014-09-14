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

        public FakeValueNode()
            : base()
        {
            this.Attributes
                .Has("value");
        }

        public override void PrePass(Kernel k)
        {
        }

        public override void PreCompile(Kernel k)
        {
        }

        public override void Compile(Kernel k)
        {
        }
    }
}
