#include "SVM_Parser.h"
#include "SVM_VirtualMachine.h"

#include <algorithm>

namespace Shogun
{
	namespace Assembler
	{
		Parser::Parser()
		{
		}

		NodeList Parser::parse(TokenStream& tokens)
		{
			NodeList nodes;

			for (TokenStream::iterator it = tokens.begin(); it != tokens.end(); ++it)
			{
				Token token = *it;
				switch (token.type)
				{
				default:
					throw ParseException(token, FORMAT("unexpected token %s", tokenTypeToString(token.type)));

				case TokenType::IDENT:
				{
					++it;
					if (it != tokens.end() && it->type == TokenType::LABEL)
					{
						// TODO: label support
						throw ParseException("Labels are not yet implemented");
					}
					else
					{
						--it;
						parseOperation(tokens, it, nodes);
					}
				}

				case TokenType::NEWLINE:
					continue;

				case TokenType::END:
					return nodes;
				}
			}

			return nodes;
		}

		void Parser::parseOperation(TokenStream& tokens, TokenStream::iterator& it, NodeList& nodes)
		{
			Token token = *it;
			String value = token.value;
			std::transform(value.begin(), value.end(), value.begin(), ::toupper);
			OperationNode* node = new OperationNode();

			try
			{
				node->setOpcode(stringToOpcode(value));
			}
			catch (InvalidOperationException& e)
			{
				throw ParseException(token, "unknown opcode", e);
			}

			while (it != tokens.end())
			{
				++it;
				token = *it;
				switch (token.type)
				{
				default:
					nodes.push_back(node);
					--it;
					return;

				case NUMBER:
				{
					ObjectPtr object = createObject(token.value);
					object->setNativeType(Object::NUMBER);
					node->getArguments().push_back(object);
					break;
				}

				case ADDRESS:
				{
					ObjectPtr object = createObject(token.value);
					object->setNativeType(Object::ADDRESS);
					node->getArguments().push_back(object);
					break;
				}

				case STRING:
				{
					ObjectPtr object = createObject(token.value);
					node->getArguments().push_back(object);
					break;
				}
				}
			}
		}
	}
}