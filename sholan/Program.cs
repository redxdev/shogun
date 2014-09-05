using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using sholan.Compiler;
using sholan.Compiler.Nodes;

namespace sholan
{
    public class Program
    {
        static void Main(string[] args)
        {
            Kernel kernel = new Kernel();

            TreeNode root = new TreeNode();
            root.Children.AddLast(
                new ExternalFunctionNode()
                    {
                        SymbolName = "print"
                    }
                );
            root.Children.AddLast(
                new FunctionCallNode()
                    {
                        Function = "print",
                        Arguments = new List<string>(new string[] {"\"Hello World!\""})
                    }
                );

            kernel.Compile(root);
            kernel.EndCompile();
            kernel.Write("test.shasm");
        }
    }
}
