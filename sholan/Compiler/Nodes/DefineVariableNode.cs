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

        public ICompileNode Value
        {
            get;
            set;
        }

        public DefineVariableNode(int line, int col)
            : base(line, col)
        {
            this.Attributes
                .Has("define-var");
        }

        public override void PrePass(Kernel k)
        {
            base.PrePass(k);

            if(this.Value != null)
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

            if (this.Value == null)
            {
                k.Emit(Opcode.PUSHNIL).Comment = "clear memory for variable";
            }
            else
            {
                this.Value.Compile(k);
            }

            k.EmitPush(symbol.Id.ToString() + "u").Comment = "store into variable";
            k.Emit(Opcode.STLO);
        }
    }
}
