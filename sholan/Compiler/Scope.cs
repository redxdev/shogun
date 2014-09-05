using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sholan.Compiler
{
    public class Scope
    {
        public Dictionary<string, Symbol> Symbols
        {
            get;
            protected set;
        }

        public string Name
        {
            get;
            set;
        }

        public Scope()
        {
            this.Symbols = new Dictionary<string, Symbol>();
        }
    }
}
