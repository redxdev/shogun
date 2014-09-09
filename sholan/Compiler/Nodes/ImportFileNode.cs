using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sholan.Compiler.Nodes
{
    public class ImportFileNode : AbstractCompileNode
    {
        public string Filepath
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
            k.Import(this.Filepath);
        }
    }
}
