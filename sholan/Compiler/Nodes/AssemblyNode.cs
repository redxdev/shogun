using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sholan.Compiler.Nodes
{
    public class AssemblyNode : AbstractCompileNode
    {
        public Operation Operation
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
            k.Emit(this.Operation);
        }
    }
}
