#ifndef _SVM_ASM_NODE_H_
#define _SVM_ASM_NODE_H_

#include "SVM_Types.h"
#include "SVM_Exception.h"
#include "SVM_Object.h"
#include "SVM_Opcodes.h"
#include "SVM_VirtualMachine.h"

#include <iostream>
#include <list>
#include <unordered_map>

namespace Shogun
{
	namespace Assembler
	{
		class Node;

		typedef std::list<ObjectPtr> CompileList;
		typedef std::list<Node*> NodeList;

		struct CompileInfo
		{
			CompileList list;
		};

		class Node
		{
		public:
			virtual void prepass(CompileInfo& compile) = 0;
			virtual void compile(CompileInfo& compile) = 0;
		};

		class OperationNode : public Node
		{
		public:
			virtual void prepass(CompileInfo& compile)
			{
				if (getOpcodeArgumentCount((Opcode)opcode) != arguments.size())
				{
					throw OperationArgumentException(
						FORMAT(
						"Invalid number of arguments for %s (expected %u, got %u)",
						opcodeToString((Opcode)opcode).c_str(),
						getOpcodeArgumentCount((Opcode)opcode),
						arguments.size()
						));
				}
			}

			virtual void compile(CompileInfo& compile)
			{
				compile.list.push_back(createObject(opcode));
				compile.list.splice(compile.list.end(), arguments);
			}

			UInt32 getOpcode()
			{
				return opcode;
			}

			void setOpcode(UInt32 opcode)
			{
				this->opcode = opcode;
			}

			CompileList& getArguments()
			{
				return arguments;
			}

		private:
			UInt32 opcode;

			CompileList arguments;
		};
	}
}

#endif