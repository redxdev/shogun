using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sholan.Compiler.Nodes.Arguments
{
    public class VariableArgument : IArgument
    {
        public string SymbolName
        {
            get;
            set;
        }

        public void PushValue(Kernel k)
        {
            Symbol symbol = k.Lookup(this.SymbolName);
            switch(symbol.SType)
            {
                case Symbol.Type.Function:
                    throw new InvalidOperationException("Cannot retrieve value of a function symbol for an argument: " + this.SymbolName);
            }

            switch(symbol.SMode)
            {
                case Symbol.Mode.Extern:
                    throw new InvalidOperationException("Tried to use a symbol that was marked as both variable and extern");
            }

            uint memory = k.CurrentScope.WalkMemoryBack(symbol.SScope);
            memory -= symbol.Id;

            k.EmitPush(memory.ToString() + "u").Comment = "variable argument (" + this.SymbolName + ")";
            k.Emit(Opcode.LDNLO);
        }
    }
}
