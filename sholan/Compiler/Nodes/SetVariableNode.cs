using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sholan.Compiler.Nodes
{
    public class SetVariableNode : AbstractCompileNode
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

        public SetVariableNode()
            : base()
        {
            this.Attributes
                .Has("variable");
        }

        public override void PrePass(Kernel k)
        {
            if(this.Value != null)
            {
                this.Value.Attributes
                    .Check("value");

                this.Value.PrePass(k);
            }
        }

        public override void PreCompile(Kernel k)
        {
            if (this.Value != null)
            {
                this.Value.PreCompile(k);
            }
        }

        public override void Compile(Kernel k)
        {
            Symbol symbol = k.Lookup(this.VariableName);

            if(this.Value == null)
            {
                k.Emit(Opcode.PUSHNIL).Comment = "set variable to nil";
            }
            else
            {
                this.Value.Compile(k);
            }

            if (symbol.SScope == k.CurrentScope)
            {
                k.EmitPush(symbol.Id.ToString() + "u").Comment = "store into variable " + this.VariableName;
                k.Emit(Opcode.STLO);
            }
            else
            {
                uint mem = k.CurrentScope.WalkMemoryBack(symbol.SScope);
                mem -= symbol.Id;

                k.EmitPush(mem.ToString() + "u").Comment = "store into variable " + this.VariableName;
                k.Emit(Opcode.STNLO);
            }
        }
    }
}
