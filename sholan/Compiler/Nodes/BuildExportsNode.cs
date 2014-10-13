using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sholan.Compiler.Nodes
{
    public class BuildExportsNode : AbstractCompileNode
    {
        public BuildExportsNode(int line, int col)
            : base(line, col)
        {
        }

        public override void PrePass(Kernel k)
        {
            base.PrePass(k);
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
