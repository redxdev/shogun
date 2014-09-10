using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sholan.Compiler.Nodes.Arguments
{
    public interface IArgument
    {
        void PushValue(Kernel k);
    }
}
