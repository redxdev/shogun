using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sholan.Compiler.Nodes
{
    public class ExpressionNode : AbstractCompileNode
    {
        public List<ICompileNode> Values
        {
            get;
            set;
        }

        public List<Opcode> Ops
        {
            get;
            set;
        }

        public ExpressionNode()
            : base()
        {
            this.Attributes
                .Has("value")
                .Has("math");
        }

        public override void PrePass(Kernel k)
        {
            foreach (ICompileNode node in this.Values)
            {
                node.Attributes
                    .Check("value");
            }

            foreach (ICompileNode node in this.Values)
            {
                node.PrePass(k);
            }
        }

        public override void PreCompile(Kernel k)
        {
            foreach (ICompileNode node in this.Values)
            {
                node.PrePass(k);
            }
        }

        public override void Compile(Kernel k)
        {
            int op = 0;
            for (int i = 0; i < this.Values.Count; i++)
            {
                this.Values[i].Compile(k);

                if(i > 0)
                {
                    k.Emit(Opcode.SWAP);
                    k.Emit(this.Ops[op]);
                    op++;
                }
            }
        }
    }
}
