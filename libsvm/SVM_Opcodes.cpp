#include "SVM_Opcodes.h"
#include "SVM_VirtualMachine.h"
#include "SVM_Exception.h"

#define OPCODE(op) case Opcode::op:
#define OPCODE_END break;

namespace Shogun
{
	void executeOperation(VirtualMachine* vm, Opcode opcode)
	{
		switch (opcode)
		{
		default:
			throw InvalidOperationException(FORMAT("Unknown opcode %u"));

			OPCODE(NOOP)
			OPCODE_END

			OPCODE(PUSH)
			{
				UInt32 i = vm->getRegPri();
				i++;
				vm->setRegPri(i);
				vm->push(vm->getMemory().get(i));
			}
			OPCODE_END

			OPCODE(POP)
			{
				vm->pop();
			}
			OPCODE_END

			OPCODE(PMMX)
			{
				vm->push(createObject(vm->getRegMmx()));
			}
			OPCODE_END

			OPCODE(PPRI)
			{
				vm->push(createObject(vm->getRegPri()));
			}
			OPCODE_END

			OPCODE(SMMX)
			{
				vm->setRegMmx(vm->pop()->getAddress());
			}
			OPCODE_END

			OPCODE(SPRI)
			{
				vm->setRegPri(vm->pop()->getAddress());
			}
			OPCODE_END

			OPCODE(HALT)
			{
				vm->halt();
			}
			OPCODE_END
		}
	}
}

#undef OPCODE
#undef OPCODE_END