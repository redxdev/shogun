using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sholan.Compiler.Nodes
{
    public class ForLoopNode : AbstractCompileNode
    {
        public ICompileNode Init
        {
            get;
            set;
        }

        public ICompileNode Check
        {
            get;
            set;
        }

        public ICompileNode Increment
        {
            get;
            set;
        }

        public ICompileNode Body
        {
            get;
            set;
        }

        public ForLoopNode()
            : base()
        {
            this.Attributes
                .Has("loop")
                .Has("for");
        }

        public override void PrePass(Kernel k)
        {
            this.Check.Attributes
                .Check("value");

            if (this.Init != null)
                this.Init.PrePass(k);

            this.Check.PrePass(k);

            if (this.Increment != null)
                this.Increment.PrePass(k);

            if (this.Body != null)
                this.Body.PrePass(k);
        }

        public override void PreCompile(Kernel k)
        {
            if (this.Init != null)
                this.Init.PreCompile(k);

            this.Check.PreCompile(k);

            if (this.Increment != null)
                this.Increment.PreCompile(k);

            if (this.Body != null)
                this.Body.PreCompile(k);
        }

        public override void Compile(Kernel k)
        {
            k.CurrentScope.PushMemory(k);

            Scope scope = k.PushScope();
            scope.Name = "for" + scope.Parent.RequestLabelId();

            if (this.Init != null)
                this.Init.Compile(k);

            string forLabel = "sl_fl_" + k.GetScopeName();
            string endLabel = "sl_flh_" + k.GetScopeName();

            Symbol breakSymbol = new Symbol()
                {
                    Name = "+break",
                    AsmName = endLabel
                };
            k.RegisterSymbol(breakSymbol);

            k.Emit(Opcode.LABEL, forLabel).Comment = "for loop";

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

            if (this.Increment != null)
                this.Increment.Compile(k);

            k.Emit(Opcode.GOTO, '"' + forLabel + '"');
            k.Emit(Opcode.LABEL, endLabel).Comment = "end for";

            k.PopScope();

            k.CurrentScope.PopMemory(k);
        }
    }
}
