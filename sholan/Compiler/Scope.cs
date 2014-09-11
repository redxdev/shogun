using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sholan.Compiler
{
    public enum ScopeType
    {
        Free,
        Function,
        Block
    }

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

        public uint MemorySpace
        {
            get;
            set;
        }

        public Scope Parent
        {
            get;
            set;
        }

        public ScopeType Type
        {
            get;
            set;
        }

        private uint currentId = 0;

        private uint currentReturn = 0;

        public Scope()
        {
            this.Symbols = new Dictionary<string, Symbol>();
            this.Type = ScopeType.Free;
        }

        public uint RequestId()
        {
            return currentId++;
        }

        public uint RequestReturn()
        {
            return currentReturn++;
        }

        public uint WalkMemoryBack(Scope until)
        {
            uint memory = 0;
            Scope current = this;
            while(current != until)
            {
                if (current == null)
                    throw new InvalidOperationException("Reached top-level scope without finding needle while walking memory");

                memory += current.MemorySpace;
                current = current.Parent;
            }

            return memory;
        }
    }
}
