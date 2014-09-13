using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sholan.Compiler.Nodes
{
    public class ReturnNode : AbstractCompileNode
    {
        public ICompileNode Value
        {
            get;
            set;
        }

        public ReturnNode()
            : base()
        {
            this.Attributes
                .Has("return")
                .Has("scope-end");
        }

        public override void PrePass(Kernel k)
        {
            if (this.Value != null)
            {
                this.Value.Attributes
                    .Check("value");

                this.Value.PrePass(k);
            }
        }

        public override void PreCompile(Kernel k)
        {
            if(this.Value != null)
            {
                this.Value.PreCompile(k);
            }
        }

        public override void Compile(Kernel k)
        {
            Symbol returnSymbol = k.Lookup("+return");

            if (this.Value != null)
                this.Value.Compile(k);
            else
                k.Emit(Opcode.PUSHNIL);

            new RetrieveVariableNode() { VariableName = "+return" }.Compile(k);

            k.EmitPush(k.CurrentScope.MemorySpace + "u").Comment = "deallocate function parameter memory";
            k.Emit(Opcode.DEALLOC);

            k.Emit(Opcode.JUMP).Comment = "return from function";
        }
    }
}
