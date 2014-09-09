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

            if(k.CurrentScope.UseReturn)
            {
                k.CurrentScope.UseReturn = false;
            }
            else
            {
                k.Emit(Opcode.POP).Comment = "remove unused return value";
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
            for(int i = 0; i < this.Arguments.Count; i++)
            {
                k.EmitPush(this.Arguments[i]).Comment = "argument " + i.ToString();
            }

            uint returnId = k.CurrentScope.RequestReturn();

            k.Emit(Opcode.PLABL, "\"sl_r_" + k.GetScopeName() + "_" + returnId.ToString() + "\"").Comment = "call function " + this.Function;
            k.Emit(Opcode.PMMX);
            k.EmitPush(k.CurrentScope.MemorySpace.ToString() + "u");
            k.Emit(Opcode.AADD);
            k.Emit(Opcode.SMMX);
            k.Emit(Opcode.GOTO, '"' + k.Lookup(this.Function).AsmName + '"');

            k.Emit(Opcode.LABEL, "sl_r_" + k.GetScopeName() + "_" + returnId.ToString()).Comment = "return point from " + this.Function;
            k.EmitPush(k.CurrentScope.MemorySpace.ToString() + "u");
            k.Emit(Opcode.PMMX);
            k.Emit(Opcode.ASUB);
            k.Emit(Opcode.SMMX);
        }
    }
}
