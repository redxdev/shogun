using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sholan.Compiler.Nodes
{
    public interface ICompileNode
    {
        int Line
        {
            get;
        }

        int Column
        {
            get;
        }

        string File
        {
            get;
        }

        bool UseReturn
        {
            get;
            set;
        }

        NodeAttributes Attributes
        {
            get;
        }

        void PrePass(Kernel k);

        void PreCompile(Kernel k);

        void Compile(Kernel k);
    }
}
