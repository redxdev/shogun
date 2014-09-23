#include "SVM_Lexer.h"

#include <regex>

namespace Shogun
{
	namespace Assembler
	{
		const char* tokenTypeToString(TokenType type)
		{
			switch (type)
			{
			case NEWLINE:
				return "NEWLINE";

			case IDENT:
				return "IDENT";

			case LABEL:
				return "LABEL";

			case NUMBER:
				return "NUMBER";

			case ADDRESS:
				return "ADDRESS";

			case STRING:
				return "STRING";

			case DIRECTIVE:
				return "DIRECTIVE";

			case END:
				return "END";
			}

			return "UNKNOWN";
		}

		Lexer::Lexer()
		{
		}

		TokenStream Lexer::tokenize(std::istream& stream)
		{
			this->currentLine = 0;
			this->currentCol = 0;

			TokenStream tokens;
			read(tokens, stream);

			return tokens;
		}

		void Lexer::read(TokenStream& tokens, std::istream& stream)
		{
			while (!stream.eof())
			{
				currentCol = 0;
				++currentLine;
				String line;
				std::getline(stream, line);

				for (String::iterator it = line.begin(); it != line.end(); ++it)
				{
					++currentCol;
					char current = *it;

					if (isspace(current)) // ignore whitespace
					{
						continue;
					}
					else if (current == ';') // comment: ignore rest of line
					{
						break;
					}
					else if (current == ':') // LABEL token
					{
						tokens.push_back(Token(TokenType::LABEL, ":", currentLine, currentCol));
					}
					else if (current == '#')
					{
						tokens.push_back(Token(TokenType::DIRECTIVE, "#", currentLine, currentCol));
					}
					else if (current == '"') // STRING token
					{
						String value;
						char last;
						UInt32 col = currentCol;
						++it;
						++currentCol;

						while (it != line.end())
						{
							last = current;
							current = *it;
							if (last == '\\')
							{
								switch (current)
								{
								case 'n':
									current = '\n';
									break;

								case 'r':
									current = '\r';
									break;

								case 't':
									current = '\t';

								case '\\':
									current = '\\';
								}

								value += current;
								current = '\0';
							}
							else if (current == '"')
							{
								break;
							}
							else if (current != '\\')
							{
								value += current;
							}

							++it;
							++currentCol;
						}

						tokens.push_back(Token(TokenType::STRING, value, col, currentLine));
					}
					else if (current == '-' || isdigit(current)) // numerics
					{
						String value;
						value += current;
						UInt32 col = currentCol;
						++it;
						++currentCol;
						bool isAddress = false;

						while (it != line.end())
						{
							current = *it;
							if (!isdigit(current))
							{
								if (current == 'u') // ADDRESS token
								{
									isAddress = true;
									++it;
									++currentCol;
									break;
								}
								else if (current == '.')
								{
									// noop
								}
								else
								{
									break;
								}
							}

							value += current;
							++it;
							++currentCol;
						}

						tokens.push_back(
							Token(isAddress ? TokenType::ADDRESS : TokenType::NUMBER, value, currentLine, col));

						--it;
						--currentCol;
					}
					else if (isalpha(current)) // IDENT
					{
						String value;
						value += current;
						UInt32 col = currentCol;
						++it;
						++currentCol;
						while (it != line.end())
						{
							current = *it;
							if (!isalnum(current) && current != '_' && current != '+')
							{
								break;
							}

							value += current;
							++it;
							++currentCol;
						}

						tokens.push_back(Token(TokenType::IDENT, value, currentLine, col));

						--it;
						--currentCol;
					}
					else
					{
						throw LexerException(FORMAT("Unexpected character %c at %u:%u", current, currentLine, currentCol));
					}
				}

				tokens.push_back(Token(TokenType::NEWLINE, "\n", currentLine, currentCol + 1));
			}

			tokens.push_back(Token(TokenType::END, "", currentLine, currentCol));
		}
	}
}