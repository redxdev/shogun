using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sholan.Compiler.Nodes
{
    public class TreeNode : AbstractCompileNode
    {
        public LinkedList<ICompileNode> Children
        {
            get;
            set;
        }

        public TreeNode(int line, int col)
            : base(line, col)
        {
            this.Children = new LinkedList<ICompileNode>();
        }

        public override void PrePass(Kernel k)
        {
            base.PrePass(k);

            foreach(ICompileNode node in this.Children)
            {
                node.PrePass(k);
            }
        }

        public override void PreCompile(Kernel k)
        {
            foreach (ICompileNode node in this.Children)
            {
                node.PreCompile(k);
            }
        }

        public override void Compile(Kernel k)
        {
            foreach (ICompileNode node in this.Children)
            {
                node.Compile(k);
            }
        }
    }
}
