#ifndef _SVM_ASM_NODE_H_
#define _SVM_ASM_NODE_H_

#include "SVM_Types.h"
#include "SVM_Exception.h"
#include "SVM_Object.h"
#include "SVM_Opcodes.h"

#include <iostream>
#include <list>

namespace Shogun
{
	namespace Assembler
	{
		class Node;

		typedef std::list<ObjectPtr> CompileList;
		typedef std::list<Node*> NodeList;

		class Node
		{
		public:
			virtual void compile(CompileList& compile) = 0;
		};

		class OperationNode : public Node
		{
		public:
			virtual void compile(CompileList& compile)
			{
				compile.push_back(createObject(opcode));
				compile.splice(compile.end(), arguments);
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