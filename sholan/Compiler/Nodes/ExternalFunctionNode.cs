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

        public List<string> Arguments
        {
            get;
            set;
        }

        public ExternalFunctionNode()
            : base()
        {
            this.Attributes
                .Has("function")
                .Has("return");
        }

        public override void PrePass(Kernel k)
        {
        }

        public override void PreCompile(Kernel k)
        {
        }

        public override void Compile(Kernel k)
        {
            Symbol symbol = new Symbol()
            {
                SMode = Symbol.Mode.Extern,
                SType = Symbol.Type.Function,
                Name = this.SymbolName,
                Id = (uint)this.Arguments.Count
            };

            k.RegisterSymbol(symbol);
        }
    }
}
