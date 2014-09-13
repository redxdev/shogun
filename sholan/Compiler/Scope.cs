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

        private uint currentLabelId = 0;

        public Scope()
        {
            this.Symbols = new Dictionary<string, Symbol>();
            this.Type = ScopeType.Free;
        }

        public uint RequestId()
        {
            return currentId++;
        }

        public uint RequestLabelId()
        {
            return currentLabelId++;
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

            memory += current.MemorySpace;

            return memory;
        }

        public void PushMemory(Kernel k)
        {
            if (this.MemorySpace == 0)
                return;

            k.Emit(Opcode.PMMX);
            k.EmitPush(this.MemorySpace.ToString() + "u");
            k.Emit(Opcode.AADD);
            k.Emit(Opcode.SMMX);
        }

        public void PopMemory(Kernel k)
        {
            if (this.MemorySpace == 0)
                return;

            k.EmitPush(this.MemorySpace.ToString() + "u");
            k.Emit(Opcode.PMMX);
            k.Emit(Opcode.ASUB);
            k.Emit(Opcode.SMMX);
        }
    }
}
