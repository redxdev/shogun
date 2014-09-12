using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sholan.Compiler.Nodes
{
    public class InternalFunctionNode : AbstractCompileNode
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

        public ICompileNode Body
        {
            get;
            set;
        }

        public InternalFunctionNode()
            : base()
        {
            this.Attributes
                .Has("function")
                .Has("return");
        }

        public override void PrePass(Kernel k)
        {
            if (this.Body != null)
                this.Body.PrePass(k);
        }

        public override void PreCompile(Kernel k)
        {
            if (this.Body != null)
                this.Body.PreCompile(k);
        }

        public override void Compile(Kernel k)
        {
            Symbol symbol = new Symbol()
                {
                    Name = this.Function,
                    SMode = Symbol.Mode.Intern,
                    SType = Symbol.Type.Function,
                    Id = (uint)this.Arguments.Count
                };

            k.RegisterSymbol(symbol);

            Scope scope = k.PushScope();
            scope.Name = this.Function;
            scope.Type = ScopeType.Function;
            scope.MemorySpace += (uint)this.Arguments.Count + 1;
            symbol.AsmName = string.Format("sl_f_{0}", k.GetScopeName());

            k.Emit(Opcode.LABEL, symbol.AsmName).Comment = "function " + this.Function; // function label

            scope.Parent.PushMemory(k);

            k.EmitPush(scope.MemorySpace.ToString() + "u").Comment = "allocate function parameter memory"; // allocate memory space for arguments and return location
            k.Emit(Opcode.ALLOC);

            Symbol returnSymbol = new Symbol()
                {
                    Name = "+return",
                    SMode = Symbol.Mode.Intern,
                    SType = Symbol.Type.Variable,
                    Id = k.CurrentScope.RequestId()
                };
            k.RegisterSymbol(returnSymbol);

            k.EmitPush(returnSymbol.Id.ToString() + "u").Comment = "store return location"; // store the return location
            k.Emit(Opcode.STLO);

            foreach(string arg in this.Arguments)
            {
                Symbol argSymbol = new Symbol()
                    {
                        Name = arg,
                        SMode = Symbol.Mode.Intern,
                        SType = Symbol.Type.Variable,
                        Id = k.CurrentScope.RequestId()
                    };
                k.RegisterSymbol(argSymbol);

                k.EmitPush(argSymbol.Id.ToString() + "u").Comment = "store argument " + arg; // store the argument
                k.Emit(Opcode.STLO);
            }

            k.Emit(Opcode.NOOP).Comment = "function body";

            if (this.Body != null)
                this.Body.Compile(k);

            new ReturnNode().Compile(k);

            scope.Parent.PopMemory(k);

            k.PopScope();

            k.Emit(Opcode.NOOP).Comment = "end of function";
        }
    }
}
