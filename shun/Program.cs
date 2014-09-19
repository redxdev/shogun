using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace shun
{
    class Program
    {
        static void Main(string[] args)
        {
            if(args.Length == 0 || args.Length > 2)
            {
                Console.WriteLine("Usage:");
                Console.WriteLine("shun <input> [output_directory=output]");
                return;
            }

            string inputFile = args[0];
            string outputDirectory = args.Length == 2 ? args[1] : "output";

            if(!File.Exists(inputFile))
            {
                Console.WriteLine("Unknown input file \"{0}\"", inputFile);
                return;
            }

            if (!Directory.Exists(outputDirectory))
            {
                Console.WriteLine("Creating output directory \"{0}\"", outputDirectory);
                try
                {
                    Directory.CreateDirectory(outputDirectory);
                }
                catch(IOException e)
                {
                    Console.WriteLine(e);
                    return;
                }
            }

            string shasmFile = Path.Combine(outputDirectory, Path.GetFileNameWithoutExtension(inputFile) + ".shasm");

            Console.WriteLine("Running SHOLAN");
            try
            {
                Process proc = Process.Start("sholan", string.Format("\"{0}\" \"{1}\"", inputFile, shasmFile));
                while (!proc.StandardOutput.EndOfStream)
                {
                    Console.WriteLine("> " + proc.StandardOutput.ReadLine());
                }
            }
            catch(Exception e)
            {
                Console.WriteLine(e);
                return;
            }

            string sxFile = Path.Combine(outputDirectory, Path.GetFileNameWithoutExtension(inputFile) + ".sx");

            Console.WriteLine("Running SHASM");
            try
            {
                Process proc = Process.Start("shasm", string.Format("-i \"{0}\" -o \"{1}\"", shasmFile, sxFile));
                while (!proc.StandardOutput.EndOfStream)
                {
                    Console.WriteLine("> " + proc.StandardOutput.ReadLine());
                }
            }
            catch (Exception e)
            {
                Console.WriteLine(e);
                return;
            }

            Console.WriteLine("Running SHOGUN");
            try
            {
                Process proc = Process.Start("shogun", string.Format("\"{0}\"", sxFile));
                while (!proc.StandardOutput.EndOfStream)
                {
                    Console.WriteLine(proc.StandardOutput.ReadLine());
                }
            }
            catch (Exception e)
            {
                Console.WriteLine(e);
                return;
            }
        }
    }
}
