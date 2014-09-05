using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sholan.Compiler.Nodes
{
    public class ExternalFunctionNode : AbstractCompileNode
    {
        public string SymbolName
        {
            get;
            set;
        }

        public void PrePass(Kernel k)
        {
            Symbol symbol = new Symbol()
                {
                    SMode = Symbol.Mode.Extern,
                    SType = Symbol.Type.Function,
                    Name = this.SymbolName
                };

            k.CurrentScope.Symbols.Add(this.SymbolName, symbol);
        }

        public void PreCompile(Kernel k)
        {
        }

        public void Compile(Kernel k)
        {
        }
    }
}
