using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sholan.Compiler.Nodes
{
    public class EntryNode : AbstractCompileNode
    {
        public ICompileNode Body
        {
            get;
            set;
        }

        public EntryNode()
            : base()
        {
            this.Attributes
                .Has("entry");
        }

        public override void PrePass(Kernel k)
        {
            if (this.Body != null)
                this.Body.PrePass(k);
        }

        public override void PreCompile(Kernel k)
        {
            k.HasEntry = true;

            if (this.Body != null)
                this.Body.PreCompile(k);
        }

        public override void Compile(Kernel k)
        {
            k.Emit(Opcode.LABEL, "sl_k_entry").Comment = "program entry point";

            Scope scope = k.PushScope();
            scope.Name = "k_entry";
            scope.Type = ScopeType.Block;

            scope.Parent.PushMemory(k);

            if (this.Body != null)
                this.Body.Compile(k);

            k.EmitPush(k.CurrentScope.MemorySpace.ToString() + "u").Comment = "deallocate memory";
            k.Emit(Opcode.DEALLOC);

            scope.Parent.PopMemory(k);

            k.PopScope();

            k.Emit(Opcode.HALT).Comment = "end entry";
        }
    }
}
