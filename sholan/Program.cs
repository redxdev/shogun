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
                new AssemblyNode()
                {
                    Operation = new Operation()
                    {
                        Op = Opcode.GOTO,
                        Argument = "\"sl_main\"",
                        Comment = "go to main method"
                    }
                }
                );
            root.Children.AddLast(
                new InternalFunctionNode()
                {
                    Function = "test_func",
                    Arguments = new List<string>(new string[] { "a", "b" }),
                    Children = new LinkedList<ICompileNode>(new ICompileNode[] { 
                        new AssemblyNode()
                        {
                            Operation = new Operation() { Op = Opcode.PUSH, Argument = "\"test\""}
                        }
                    })
                }
                );
            root.Children.AddLast(
                new HaltNode()
                );
            root.Children.AddLast(
                new AssemblyNode()
                {
                    Operation = new Operation() { Op = Opcode.LABEL, Argument = "sl_main"}
                }
                );
            root.Children.AddLast(
                new FunctionCallNode()
                {
                    Function = "test_func",
                    Arguments = new List<string>(new string[] { "\"foo\"", "\"bar\"" })
                }
                );

            kernel.Compile(root);
            kernel.EndCompile();
            kernel.Write("test.shasm");
        }
    }
}
