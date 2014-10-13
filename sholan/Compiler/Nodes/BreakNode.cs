using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sholan.Compiler.Nodes
{
    public class BreakNode : AbstractCompileNode
    {
        public BreakNode(int line, int col)
            : base(line, col)
        {
            this.Attributes
                .Has("break");
        }

        public override void PrePass(Kernel k)
        {
            base.PrePass(k);
        }

        public override void PreCompile(Kernel k)
        {
        }

        public override void Compile(Kernel k)
        {
            Symbol breakSymbol = k.Lookup("+break");
            uint mem = 0;
            Scope current = k.CurrentScope;
            while (current != breakSymbol.SScope)
            {
                mem += current.MemorySpace;
                current.PopMemory(k, false);

                current = current.Parent;
            }

            current.PopMemory(k, false);

            k.EmitPush(mem + "u").Comment = "deallocate scope memory";
            k.Emit(Opcode.DEALLOC);

            k.Emit(Opcode.GOTO, "\"" + breakSymbol.AsmName + "\"").SetDebug(Line, Column, DebugType.Break, "");
        }
    }
}
