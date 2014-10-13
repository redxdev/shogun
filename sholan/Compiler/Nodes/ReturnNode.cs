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

        public ReturnNode(int line, int col)
            : base(line, col)
        {
            this.Attributes
                .Has("return");
        }

        public override void PrePass(Kernel k)
        {
            base.PrePass(k);

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

            var rvn = new RetrieveVariableNode(-1, -1) {VariableName = "+return"};
            rvn.PrePass(k);
            rvn.PreCompile(k);
            rvn.Compile(k);

            uint mem = 0;
            Scope current = k.CurrentScope;
            while(current != returnSymbol.SScope)
            {
                mem += current.MemorySpace;
                current.PopMemory(k, false);

                current = current.Parent;
            }

            mem += current.MemorySpace;

            current.PopMemory(k, false);

            k.EmitPush(mem + "u").Comment = "deallocate function memory";
            k.Emit(Opcode.DEALLOC);

            k.Emit(Opcode.JUMP).SetDebug(File, Line, Column, DebugType.Return, "");
        }
    }
}
