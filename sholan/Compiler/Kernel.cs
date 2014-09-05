using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sholan.Compiler
{
    public class Kernel
    {
        private Stack<Scope> scopeStack = new Stack<Scope>();

        public Scope CurrentScope
        {
            get
            {
                return scopeStack.Peek();
            }
        }

        public Kernel()
        {
            PushScope().Name = "k";
        }

        public Scope PushScope()
        {
            Scope scope = new Scope();
            scopeStack.Push(scope);
            return scope;
        }

        public Scope PopScope()
        {
            return scopeStack.Pop();
        }

        public Symbol Lookup(string symbolName)
        {
            foreach(Scope scope in scopeStack)
            {
                if(scope.Symbols.ContainsKey(symbolName))
                {
                    return scope.Symbols[symbolName];
                }
            }

            throw new KeyNotFoundException(string.Format("Unknown symbol \"{0}\"", symbolName));
        }
    }
}
