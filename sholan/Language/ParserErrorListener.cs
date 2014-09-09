using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Antlr4.Runtime;

namespace sholan.Language
{
    public class ParserErrorListener : BaseErrorListener
    {
        public static readonly ParserErrorListener Instance = new ParserErrorListener();

        public override void SyntaxError(IRecognizer recognizer, IToken offendingSymbol, int line, int charPositionInLine, string msg, RecognitionException e)
        {
            throw new ParseException(string.Format("Syntax error at {0}:{1} - {2}", line, charPositionInLine, msg));
        }
    }
}