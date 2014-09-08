using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sholan.Compiler.Nodes
{
    public class InternalFunctionNode : TreeNode
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

        public override void Compile(Kernel k)
        {
            Symbol symbol = new Symbol()
                {
                    Name = this.Function,
                    SMode = Symbol.Mode.Intern,
                    SType = Symbol.Type.Function
                };

            k.RegisterSymbol(symbol);

            k.Emit(Opcode.LABEL, string.Format("_sl_f_{0}", this.Function)).Comment = "function " + this.Function; // function label

            Scope scope = k.PushScope();

            k.EmitPush((this.Arguments.Count + 1).ToString() + "u").Comment = "allocate function parameter memory"; // allocate memory space for arguments and return location
            k.Emit(Opcode.ALLOC);
            scope.MemorySpace += (uint)this.Arguments.Count + 1;

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

            base.Compile(k);
            k.PopScope();

            k.EmitPush(returnSymbol.Id.ToString() + "u").Comment = "get return location";
            k.Emit(Opcode.LDLO);

            k.EmitPush((this.Arguments.Count + 1).ToString() + "u").Comment = "deallocate function parameter memory";
            k.Emit(Opcode.DEALLOC);

            k.Emit(Opcode.JUMP).Comment = "return from function";
        }
    }
}
