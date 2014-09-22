using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using sholan.Compiler.Nodes;
using sholan.Language;

namespace sholan.Compiler
{
    public enum ImportMode
    {
        None,
        Import,
        Inherit,
        Library,
        Export
    }

    public class Kernel
    {
        private Stack<Scope> scopeStack = new Stack<Scope>();

        private List<Operation> operations = new List<Operation>();

        private bool hasEntry = false;

        private HashSet<string> importedFiles = new HashSet<string>();

        private HashSet<string> importPaths = new HashSet<string>();

        private LinkedList<ImportMode> importModes = new LinkedList<ImportMode>();

        private List<Symbol> exports = new List<Symbol>();

        public Scope CurrentScope
        {
            get
            {
                return scopeStack.Peek();
            }
        }

        public List<Operation> Operations
        {
            get
            {
                return this.operations;
            }
        }

        public ImportMode CurrentImportMode
        {
            get
            {
                if (importModes.Count == 0)
                    return ImportMode.None;

                LinkedListNode<ImportMode> current = importModes.First;
                while(true)
                {
                    switch(current.Value)
                    {
                        case ImportMode.None:
                            return ImportMode.None;

                        case ImportMode.Import:
                            return ImportMode.Import;

                        case ImportMode.Library:
                            return ImportMode.Library;

                        case ImportMode.Export:
                            return ImportMode.Export;
                    }

                    if (current.Next == null)
                        return ImportMode.Import;

                    current = current.Next;
                }
            }
        }

        public List<Symbol> Exports
        {
            get
            {
                return this.exports;
            }
        }

        public Kernel()
        {
            PushScope().MemorySpace = 1;
            this.CurrentScope.PushMemory(this);
        }

        public Scope PushScope()
        {
            return this.PushScope(new Scope());
        }

        public Scope PushScope(Scope scope)
        {
            if (scopeStack.Count > 0)
                scope.Parent = scopeStack.Peek();

            scopeStack.Push(scope);
            return scope;
        }

        public Scope PopScope()
        {
            Scope scope = scopeStack.Pop();

            return scope;
        }

        public Symbol RegisterSymbol(Symbol symbol)
        {
            symbol.SScope = this.CurrentScope;
            this.CurrentScope.Symbols.Add(symbol.Name, symbol);
            return symbol;
        }

        public string GetScopeName()
        {
            string result = string.Empty;
            foreach (Scope scope in scopeStack.Reverse())
            {
                result += scope.Name + "_";
            }

            if (scopeStack.Count > 0)
                result = result.Substring(0, result.Length - 1);

            return result;
        }

        public Symbol Lookup(string symbolName)
        {
            foreach(Scope scope in scopeStack)
            {
                if(scope.Symbols.ContainsKey(symbolName))
                {
                    return scope.Symbols[symbolName];
                }
            }

            throw new KeyNotFoundException(string.Format("Unknown symbol \"{0}\"", symbolName));
        }

        public Operation Emit(Opcode o)
        {
            Operation op = new Operation()
                {
                    Op = o,
                    Argument = string.Empty
                };
            this.operations.Add(op);
            return op;
        }

        public Operation Emit(Opcode o, string argument)
        {
            Operation op = new Operation()
            {
                Op = o,
                Argument = argument
            };
            this.operations.Add(op);
            return op;
        }

        public Operation Emit(Operation o)
        {
            this.operations.Add(o);
            return o;
        }

        public Operation EmitPush(string argument)
        {
            Operation op = new Operation()
            {
                Op = Opcode.PUSH,
                Argument = argument
            };
            this.operations.Add(op);
            return op;
        }

        public void AddImportPath(string path)
        {
            string fullPath = Path.GetFullPath(path);
            this.importPaths.Add(fullPath);
        }

        public void RemoveImportPath(string path)
        {
            string fullPath = Path.GetFullPath(path);
            this.importPaths.Remove(fullPath);
        }

        public void Import(string file, ImportMode mode)
        {
            string foundFile = file;
            if(!File.Exists(foundFile))
            {
                foreach (string path in this.importPaths)
                {
                    foundFile = Path.Combine(path, file);
                    if (File.Exists(foundFile))
                        break;
                }

                if (!File.Exists(foundFile))
                    throw new CompileException(string.Format("Unable to find import \"{0}\" in import paths", file));
            }

            string fullPath = Path.GetFullPath(foundFile);
            if (importedFiles.Contains(fullPath))
                return;

            string fileDir = Path.GetDirectoryName(fullPath);
            this.AddImportPath(fileDir);

            PushImportMode(mode);

            try
            {
                Nodes.ICompileNode root = LanguageUtilities.ParseFile(fullPath);
                this.Compile(root);
            }
            catch(Exception e)
            {
                throw new CompileException("Encountered exception while including " + fullPath, e);
            }

            PopImportMode();

            this.RemoveImportPath(fileDir);
        }

        public void PushImportMode(ImportMode mode)
        {
            this.importModes.AddFirst(mode);
        }

        public void PopImportMode()
        {
            this.importModes.RemoveFirst();
        }

        public void AddExport(Symbol symbol)
        {
            this.exports.Add(symbol);
        }

        public void Compile(Nodes.ICompileNode root)
        {
            root.PrePass(this);
            root.PreCompile(this);

            if (this.CurrentImportMode == ImportMode.None)
            {
                FunctionCallNode node = new FunctionCallNode()
                    {
                        Function = "+entry",
                        Arguments = new List<ICompileNode>()
                    };
                node.Compile(this);
                this.Emit(Opcode.HALT);
            }

            root.Compile(this);
        }

        public void EndCompile(bool halt = false)
        {
            if(halt)
                this.Emit(Opcode.HALT).Comment = "end compile (halt=true)";
        }

        public void Write(string file)
        {
            using(StreamWriter writer = new StreamWriter(file))
            {
                writer.Write("; Generated by sholan compiler" + Environment.NewLine);

                foreach(Operation op in this.operations)
                {
                    writer.Write(op.GetOutput() + Environment.NewLine);
                }
            }
        }
    }
}
