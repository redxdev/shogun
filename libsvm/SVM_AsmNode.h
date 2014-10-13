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
		typedef std::unordered_map<String, UInt32> LabelMap;
		typedef std::list<Node*> NodeList;

		SVM_EXCEPTION_AUTO(LabelException, LanguageException)

		struct CompileInfo
		{
			CompileList list;
			LabelMap labels;
			UInt32 currentId = 0;
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
			virtual void prepass(CompileInfo& compile) override
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

				switch (opcode)
				{
				default:
					++compile.currentId;
					break;

				case PUSH:
				case PLABL:
					compile.currentId += 2;
					break;

				case GOTO:
				case GOTOF:
					compile.currentId += 3;
					break;
				}
			}

			virtual void compile(CompileInfo& compile) override
			{
				switch (opcode)
				{
				default:
				{
					ObjectPtr op = createObject(opcode);
					op->setDebug(this->debug);
					compile.list.push_back(op);
					compile.list.splice(compile.list.end(), arguments);
					break;
				}

				case GOTO:
				case GOTOF:
				{
					LabelMap::iterator found = compile.labels.find(arguments.begin()->get()->getString());
					if (found == compile.labels.end())
						throw LabelException(FORMAT("Unknown label %s", arguments.begin()->get()->getString().c_str()));

					UInt32 label = found->second;
					compile.list.push_back(createObject((UInt32)Opcode::PUSH));
					compile.list.push_back(createObject(label + VirtualMachine::getReservedAllocation()));
					ObjectPtr jmp = createObject((UInt32)(opcode == Opcode::GOTO ? Opcode::JUMP : Opcode::JUMPF));
					jmp->setDebug(this->debug);
					compile.list.push_back(jmp);
					break;
				}

				case PLABL:
				{
					LabelMap::iterator found = compile.labels.find(arguments.begin()->get()->getString());
					if (found == compile.labels.end())
						throw LabelException(FORMAT("Unknown label %s", arguments.begin()->get()->getString().c_str()));

					UInt32 label = found->second;
					ObjectPtr psh = createObject((UInt32)Opcode::PUSH);
					psh->setDebug(this->debug);
					compile.list.push_back(psh);
					compile.list.push_back(createObject(label + VirtualMachine::getReservedAllocation()));
					break;
				}
				}
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

			void setDebug(Object::DebugInfo* debug) {
				this->debug = debug;
			}

			Object::DebugInfo* getDebug() {
				return this->debug;
			}

		private:
			UInt32 opcode;

			CompileList arguments;

			Object::DebugInfo* debug = 0;
		};

		class LabelNode : public Node
		{
		public:
			virtual void prepass(CompileInfo& compile) override
			{
				if (compile.labels.find(label) != compile.labels.end())
					throw LabelException(FORMAT("Label %s already exists", label.c_str()));

				compile.labels[label] = compile.currentId;
				++compile.currentId;
			}

			virtual void compile(CompileInfo& compile) override
			{
				compile.list.push_back(createObject((UInt32)Opcode::NOOP));
			}

			void setLabel(String label)
			{
				this->label = label;
			}

			const String& getLabel()
			{
				return this->label;
			}

		private:
			String label;
		};
	}
}

#endif