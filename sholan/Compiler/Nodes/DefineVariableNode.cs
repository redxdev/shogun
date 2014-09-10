using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sholan.Compiler.Nodes
{
    public class DefineVariableNode : AbstractCompileNode
    {
        public string VariableName
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
            k.EmitPush("1u").Comment = "allocate variable " + this.VariableName;
            k.Emit(Opcode.ALLOC);
            k.CurrentScope.MemorySpace += 1;

            Symbol symbol = new Symbol()
                {
                    Name = this.VariableName,
                    SMode = Symbol.Mode.Intern,
                    SType = Symbol.Type.Variable,
                    Id = k.CurrentScope.RequestId()
                };

            k.RegisterSymbol(symbol);

            k.Emit(Opcode.PUSHNIL).Comment = "clear memory for variable";
            k.EmitPush(symbol.Id.ToString() + "u");
            k.Emit(Opcode.LDLO);
        }
    }
}
