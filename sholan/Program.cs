using System;
using System.Collections.Generic;
using System.IO;
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

            string file = Path.GetFullPath(args[0]);
            kernel.FileStack.Push(file);

            try
            {
                root = LanguageUtilities.ParseFile(file);
            }
            catch(Exception e)
            {
                Console.WriteLine("Error parsing input:");
                Console.WriteLine(e.ToString());
                return;
            }

            Console.WriteLine("Compiling...");
            kernel.AddImportPath(Path.GetDirectoryName(Path.GetFullPath(args[0])));
            try
            {
                kernel.Compile(root);
            }
            catch(Exception e)
            {
                Console.WriteLine("Error compiling:");
                Console.WriteLine(e.ToString());
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
                Console.WriteLine(e.ToString());
                return;
            }

            Console.WriteLine("Finished compile");
        }
    }
}
