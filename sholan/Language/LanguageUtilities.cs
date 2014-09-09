using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using sholan.Compiler.Nodes;
using Antlr4.Runtime;

namespace sholan.Language
{
    public static class LanguageUtilities
    {
        public static ICompileNode ParseString(string input)
        {
            return Parse(new AntlrInputStream(input));
        }

        public static ICompileNode ParseFile(string filename)
        {
            return Parse(new AntlrFileStream(filename));
        }

        public static ICompileNode Parse(ICharStream input)
        {
            SLangLexer lexer = new SLangLexer(input);
            lexer.RemoveErrorListeners();
            lexer.AddErrorListener(LexerErrorListener.Instance);

            CommonTokenStream tokenStream = new CommonTokenStream(lexer);

            SLangParser parser = new SLangParser(tokenStream);
            parser.RemoveErrorListeners();
            parser.AddErrorListener(ParserErrorListener.Instance);

            ICompileNode rootNode = parser.compileUnit().rootNode;

            return rootNode;
        }
    }
}