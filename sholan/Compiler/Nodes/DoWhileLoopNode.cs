using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sholan.Compiler.Nodes
{
    public class DoWhileLoopNode : AbstractCompileNode
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

        public DoWhileLoopNode(int line, int col)
            : base(line, col)
        {
            this.Attributes
                .Has("loop");
        }

        public override void PrePass(Kernel k)
        {
            base.PrePass(k);

            this.Check.Attributes
                .Check("value");

            if (this.Body != null)
                this.Body.PrePass(k);

            this.Check.PrePass(k);
        }

        public override void PreCompile(Kernel k)
        {
            if (this.Body != null)
                this.Body.PreCompile(k);

            this.Check.PreCompile(k);
        }

        public override void Compile(Kernel k)
        {
            k.CurrentScope.PushMemory(k);

            Scope scope = k.PushScope();
            scope.Name = "dowhile" + scope.Parent.RequestLabelId();

            string doWhileLabel = "sl_dwl_" + k.GetScopeName();
            string endLabel = "sl_dwlh_" + k.GetScopeName();

            Symbol breakSymbol = new Symbol()
            {
                Name = "+break",
                AsmName = endLabel
            };
            k.RegisterSymbol(breakSymbol);

            k.Emit(Opcode.LABEL, doWhileLabel).Comment = "do while loop";
            k.Emit(Opcode.NOOP).SetDebug(Line, Column, DebugType.DoLoop, "");

            scope.PushMemory(k);

            Scope innerScope = k.PushScope();
            innerScope.Name = "in";

            if (this.Body != null)
                this.Body.Compile(k);

            k.EmitPush(innerScope.MemorySpace.ToString() + "u");
            k.Emit(Opcode.DEALLOC);

            k.PopScope();

            scope.PopMemory(k);

            this.Check.Compile(k);

            k.Emit(Opcode.GOTOF, '"' + doWhileLabel + '"');
            k.Emit(Opcode.LABEL, endLabel).Comment = "end do while";

            k.PopScope();

            k.CurrentScope.PopMemory(k);
        }
    }
}
