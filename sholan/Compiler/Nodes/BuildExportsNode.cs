using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sholan.Compiler.Nodes
{
    public class BuildExportsNode : AbstractCompileNode
    {
        public override void PrePass(Kernel k)
        {
            k.IsBuildingExports = true;
        }

        public override void PreCompile(Kernel k)
        {
        }

        public override void Compile(Kernel k)
        {
        }
    }
}
