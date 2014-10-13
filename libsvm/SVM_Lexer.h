#ifndef _SVM_LEXER_H_
#define _SVM_LEXER_H_

#include "SVM_Types.h"
#include "SVM_Exception.h"
#include "SVM_StringUtils.h"

#include <list>
#include <iostream>

namespace Shogun
{
	namespace Assembler
	{
		enum TokenType
		{
			NEWLINE,
			IDENT,
			LABEL,
			NUMBER,
			ADDRESS,
			STRING,
			DIRECTIVE,
			DEBUG_STR,
			END
		};

		const char* tokenTypeToString(TokenType type);

		struct Token
		{
			Token(TokenType type, const String& value, UInt32 line, UInt32 column)
			{
				this->type = type;
				this->value = value;
				this->line = line;
				this->column = column;
			}

			TokenType type;

			String value;

			UInt32 line;

			UInt32 column;
		};

		typedef std::list<Token> TokenStream;
		
		SVM_EXCEPTION_AUTO(LanguageException, Exception);

		SVM_EXCEPTION_AUTO(LexerException, LanguageException);

		class Lexer
		{
		public:
			Lexer();

			TokenStream tokenize(std::istream& stream);

		private:
			void read(TokenStream& tokens, std::istream& stream);

		private:
			UInt32 currentLine = 0;
			UInt32 currentCol = 0;
		};
	}
}

#endif