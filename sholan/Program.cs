using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using sholan.Compiler;
using sholan.Compiler.Nodes;
using sholan.Language;

namespace sholan
{
    public class Program
    {
        static void Main(string[] args)
        {
            if(args.Length != 2)
            {
                Console.WriteLine("Usage:");
                Console.WriteLine("sholan <input> <output>");
                return;
            }

            Console.WriteLine("Creating kernel...");
            Kernel kernel = new Kernel();

            Console.WriteLine("Parsing input...");
            ICompileNode root = null;

            try
            {
                root = LanguageUtilities.ParseFile(args[0]);
            }
            catch(Exception e)
            {
                Console.WriteLine("Error parsing input:");
                Console.WriteLine(e.Message);
                Console.WriteLine(e.StackTrace);
                return;
            }

            Console.WriteLine("Compiling...");
            try
            {
                kernel.Compile(root);
            }
            catch(Exception e)
            {
                Console.WriteLine("Error compiling:");
                Console.WriteLine(e.Message);
                Console.WriteLine(e.StackTrace);
                return;
            }

            kernel.EndCompile();

            Console.WriteLine("Kernel operation count: " + kernel.Operations.Count);

            Console.WriteLine("Writing assembly...");

            try
            {
                kernel.Write(args[1]);
            }
            catch(Exception e)
            {
                Console.WriteLine("Error writing assembly:");
                Console.WriteLine(e.Message);
                Console.WriteLine(e.StackTrace);
                return;
            }

            Console.WriteLine("Finished compile");
        }
    }
}
