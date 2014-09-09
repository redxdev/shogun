using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sholan.Compiler
{
    public class Symbol
    {
        public enum Type
        {
            Invalid,
            Function,
            Variable
        }

        public enum Mode
        {
            Invalid,
            Intern,
            Extern
        }

        public Type SType
        {
            get;
            set;
        }

        public Mode SMode
        {
            get;
            set;
        }

        public string Name
        {
            get;
            set;
        }

        public string AsmName
        {
            get;
            set;
        }

        public uint Id
        {
            get;
            set;
        }

        public Scope SScope
        {
            get;
            set;
        }
    }
}
