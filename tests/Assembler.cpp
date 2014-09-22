#include <gtest/gtest.h>

#include <sstream>
#include <fstream>

#include "SVM_Lexer.h"
#include "SVM_Types.h"
#include "SVM_Parser.h"
#include "SVM_AsmNode.h"
#include "SVM_AsmWriter.h"
#include "SVM_VirtualMachine.h"

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

		const char* parserTestInput =
			"push 5\n"
			"push 10u\n"
			"push \"hello world!\"\n"
			"halt";

		const char* multiTestInput =
			"push 25.5\n"
			"halt";

		TEST(AssemblerTests, LexerTokenStream)
		{
			Lexer lexer;
			std::istringstream input(lexerTestInput);

			TokenStream tokens;
			ASSERT_NO_THROW({
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

		TEST(AssemblerTests, ParserNodeList)
		{
			Lexer lexer;
			std::istringstream input(parserTestInput);

			TokenStream tokens;
			ASSERT_NO_THROW({
				tokens = lexer.tokenize(input);
			});

			Parser parser;

			NodeList nodes;
			ASSERT_NO_THROW({
				nodes = parser.parse(tokens);
			});

			EXPECT_EQ(4, nodes.size());
		}

		TEST(AssemblerTests, AsmReadWrite)
		{
			Lexer lexer;
			std::istringstream input(parserTestInput);
			
			TokenStream tokens = lexer.tokenize(input);

			Parser parser;
			NodeList nodes = parser.parse(tokens);

			AsmWriter writer;
			std::ofstream outputFile("./test.sx", std::ios::out | std::ios::ate);

			ASSERT_NO_THROW({
				writer.write(outputFile, nodes);
			});

			outputFile.close();

			AsmReader reader;
			std::ifstream inputFile("./test.sx");
			
			CompileInfo compile;
			ASSERT_NO_THROW({
				compile = reader.read(inputFile);
			});
		}

		TEST(AssemblerTests, Multi)
		{
			Lexer lexer;
			std::istringstream input(multiTestInput);

			TokenStream tokens = lexer.tokenize(input);

			Parser parser;
			NodeList nodes = parser.parse(tokens);

			AsmWriter writer;
			std::ofstream outputFile("./test.sx", std::ios::out | std::ios::ate);

			writer.write(outputFile, nodes);
			outputFile.close();

			AsmReader reader;
			std::ifstream inputFile("./test.sx");

			CompileInfo compile;
			compile = reader.read(inputFile);

			Program program;
			program.insert(program.begin(), compile.list.begin(), compile.list.end());

			VirtualMachine vm(0);

			vm.loadProgram(program);
			vm.run();

			EXPECT_FLOAT_EQ(25.5f, vm.pop()->getNumber());
		}
	}
}