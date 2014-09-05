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
            protected set;
        }

        public TreeNode()
        {
            this.Children = new LinkedList<ICompileNode>();
        }

        public void PrePass(Kernel k)
        {
            foreach(ICompileNode node in this.Children)
            {
                node.PrePass(k);
            }
        }

        public void PreCompile(Kernel k)
        {
            foreach (ICompileNode node in this.Children)
            {
                node.PreCompile(k);
            }
        }

        public void Compile(Kernel k)
        {
            foreach (ICompileNode node in this.Children)
            {
                node.Compile(k);
            }
        }
    }
}
