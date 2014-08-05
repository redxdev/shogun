#ifndef _SVM_PARSER_H_
#define _SVM_PARSER_H_

#include "SVM_Exception.h"
#include "SVM_Types.h"
#include "SVM_Lexer.h"
#include "SVM_StringUtils.h"
#include "SVM_AsmNode.h"

namespace Shogun
{
	namespace Assembler
	{
		SVM_EXCEPTION_START(ParseException, LanguageException)
			ParseException(const Token& token, const String& error)
				: LanguageException(
				FORMAT("Parse error near \"%s\" (%u:%u) - %s", token.value.c_str(), token.line, token.column, error.c_str()))
			{
			}
		SVM_EXCEPTION_END

		class Parser
		{
		public:
			Parser();

			NodeList parse(TokenStream& tokens);

		private:
			void parseOperation(TokenStream& tokens, TokenStream::iterator& it, NodeList& nodes);
		};
	}
}

#endif