using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sholan.Compiler.Nodes
{
    public class ReturnNode : AbstractCompileNode
    {
        public string Value
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
            Scope scope = k.CurrentScope;
            Symbol returnSymbol = k.Lookup("+return");

            if (this.Value != null)
                k.EmitPush(this.Value);

            k.EmitPush(returnSymbol.Id.ToString() + "u").Comment = "get return location";
            k.Emit(Opcode.LDLO);

            k.EmitPush(scope.MemorySpace + "u").Comment = "deallocate function parameter memory";
            k.Emit(Opcode.DEALLOC);

            k.Emit(Opcode.JUMP).Comment = "return from function";
        }
    }
}
