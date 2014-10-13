using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sholan.Compiler.Nodes
{
    public class RetrieveVariableNode : AbstractCompileNode
    {
        public string VariableName
        {
            get;
            set;
        }

        public RetrieveVariableNode(int line, int col)
            : base(line, col)
        {
            this.Attributes
                .Has("variable")
                .Has("value");
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
            Symbol symbol = k.Lookup(this.VariableName);

            if (symbol.SScope == k.CurrentScope)
            {
                k.EmitPush(symbol.Id.ToString() + "u").Comment = "retrieve variable " + this.VariableName;
                k.Emit(Opcode.LDLO).SetDebug(File, Line, Column, DebugType.Retrieve, this.VariableName);
            }
            else
            {
                uint mem = k.CurrentScope.WalkMemoryBack(symbol.SScope);
                mem -= symbol.Id;

                k.EmitPush(mem.ToString() + "u").Comment = "retrieve variable " + this.VariableName;
                k.Emit(Opcode.LDNLO).SetDebug(File, Line, Column, DebugType.Retrieve, this.VariableName);
            }
        }
    }
}
