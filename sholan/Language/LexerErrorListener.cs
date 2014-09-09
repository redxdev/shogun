using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Antlr4.Runtime;

namespace sholan.Language
{
    public class LexerErrorListener : IAntlrErrorListener<int>
    {
        public static readonly LexerErrorListener Instance = new LexerErrorListener();

        public virtual void SyntaxError(IRecognizer recognizer, int offendingSymbol, int line, int charPositionInLine, string msg, RecognitionException e)
        {
            throw new ParseException(string.Format("Syntax error at {0}:{1} - {2}", line, charPositionInLine, msg));
        }
    }
}