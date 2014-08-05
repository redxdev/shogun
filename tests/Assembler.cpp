#include <gtest/gtest.h>

#include <sstream>

#include "SVM_Lexer.h"
#include "SVM_Types.h"

using namespace Shogun::Assembler;

namespace Shogun
{
	namespace Tests
	{
		const char* lexerTestInput =
			"main:\n"
			"push 5\n"
			"push 10u\n"
			"push 1.3";

		TEST(AssemblerTests, LexerTokenStream)
		{
			Lexer lexer;
			std::istringstream input(lexerTestInput);

			TokenStream tokens;
			EXPECT_NO_THROW({
				tokens = lexer.tokenize(input);
			}); 

			TokenStream::iterator it = tokens.begin();
			EXPECT_EQ(TokenType::IDENT, it->type);
			++it;
			EXPECT_EQ(TokenType::LABEL, it->type);
			++it;
			EXPECT_EQ(TokenType::NEWLINE, it->type);
			++it;
			EXPECT_EQ(TokenType::IDENT, it->type);
			++it;
			EXPECT_EQ(TokenType::NUMBER, it->type);
			++it;
			EXPECT_EQ(TokenType::NEWLINE, it->type);
			++it;
			EXPECT_EQ(TokenType::IDENT, it->type);
			++it;
			EXPECT_EQ(TokenType::ADDRESS, it->type);
			++it;
			EXPECT_EQ(TokenType::NEWLINE, it->type);
			++it;
			EXPECT_EQ(TokenType::IDENT, it->type);
			++it;
			EXPECT_EQ(TokenType::NUMBER, it->type);
			++it;
			EXPECT_EQ(TokenType::NEWLINE, it->type);
			++it;
			EXPECT_EQ(TokenType::END, it->type);
		}
	}
}