using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sholan.Compiler.Nodes
{
    public class WhileLoopNode : AbstractCompileNode
    {
        public ICompileNode Check
        {
            get;
            set;
        }

        public ICompileNode Body
        {
            get;
            set;
        }

        public WhileLoopNode()
            : base()
        {
            this.Attributes
                .Has("loop")
                .Has("while");
        }

        public override void PrePass(Kernel k)
        {
            this.Check.Attributes
                .Check("value");

            this.Check.PrePass(k);

            if (this.Body != null)
                this.Body.PrePass(k);
        }

        public override void PreCompile(Kernel k)
        {
            this.Check.PreCompile(k);

            if (this.Body != null)
                this.Body.PreCompile(k);
        }

        public override void Compile(Kernel k)
        {
            k.CurrentScope.PushMemory(k);

            Scope scope = k.PushScope();
            scope.Name = "while" + scope.Parent.RequestLabelId();

            string whileLabel = "sl_wl_" + k.GetScopeName();
            string endLabel = "sl_wlh_" + k.GetScopeName();

            Symbol breakSymbol = new Symbol()
                {
                    Name = "+break",
                    AsmName = endLabel
                };
            k.RegisterSymbol(breakSymbol);

            k.Emit(Opcode.LABEL, whileLabel).Comment = "while loop";

            this.Check.Compile(k);

            k.Emit(Opcode.NOT);
            k.Emit(Opcode.GOTOF, '"' + endLabel + '"');

            scope.PushMemory(k);

            Scope innerScope = k.PushScope();
            innerScope.Name = "in";

            if (this.Body != null)
                this.Body.Compile(k);

            k.EmitPush(innerScope.MemorySpace.ToString() + "u");
            k.Emit(Opcode.DEALLOC);

            k.PopScope();

            scope.PopMemory(k);

            k.Emit(Opcode.GOTO, '"' + whileLabel + '"');
            k.Emit(Opcode.LABEL, endLabel).Comment = "end while";

            k.PopScope();

            k.CurrentScope.PopMemory(k);
        }
    }
}
