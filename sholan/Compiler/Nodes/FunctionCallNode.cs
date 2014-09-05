using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sholan.Compiler.Nodes
{
    public class FunctionCallNode : AbstractCompileNode
    {
        public string Function
        {
            get;
            set;
        }

        public List<string> Arguments
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
            Symbol symbol = k.Lookup(this.Function);

            switch(symbol.SType)
            {
                case Symbol.Type.Function:
                    break;

                default:
                    throw new InvalidOperationException(string.Format("Tried to compile a function call on a non-function symbol \"{0}\"", this.Function));
            }

            switch(symbol.SMode)
            {
                default:
                    throw new InvalidOperationException(string.Format("Unknown symbol mode {0} on symbol \"{1}\"", symbol.SMode.ToString(), this.Function));

                case Symbol.Mode.Extern:
                    this.CompileExtern(k);
                    break;

                case Symbol.Mode.Intern:
                    this.CompileIntern(k);
                    break;
            }
        }

        protected void CompileExtern(Kernel k)
        {
            for(int i = this.Arguments.Count - 1; i >= 0; i--)
            {
                k.EmitPush(this.Arguments[i]).Comment = "argument " + i.ToString();
            }

            k.EmitPush('"' + this.Function + '"').Comment = "function name";
            k.Emit(Opcode.ECALL).Comment = "call function";
        }

        protected void CompileIntern(Kernel k)
        {
            throw new NotImplementedException("CompileIntern not yet implemented");
        }
    }
}
