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

			OPCODE(DUP)
			{
				vm->push(createObject(vm->peek().get()));
			}
			OPCODE_END

			OPCODE(REF)
			{
				vm->push(vm->peek());
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

			OPCODE(ALLOC)
			{
				UInt32 amount = vm->pop()->getAddress();
				vm->getMemory().resize(vm->getMemory().getSize() + amount);
			}
			OPCODE_END

			OPCODE(DEALLOC)
			{
				UInt32 amount = vm->pop()->getAddress();
				vm->getMemory().resize(vm->getMemory().getSize() - amount);
			}
			OPCODE_END

			OPCODE(STORE)
			{
				UInt32 address = vm->pop()->getAddress();
				ObjectPtr data = vm->pop();
				vm->getMemory().set(address, data);
			}
			OPCODE_END

			OPCODE(LOAD)
			{
				UInt32 address = vm->pop()->getAddress();
				vm->push(vm->getMemory().get(address));
			}
			OPCODE_END

			OPCODE(STLO)
			{
				UInt32 address = vm->pop()->getAddress();
				ObjectPtr data = vm->pop();
				vm->getMemory().set(vm->getRegMmx() + address, data);
			}
			OPCODE_END

			OPCODE(LDLO)
			{
				UInt32 address = vm->pop()->getAddress();
				vm->push(vm->getMemory().get(vm->getRegMmx() + address));
			}
			OPCODE_END
		}
	}
}

#undef OPCODE
#undef OPCODE_END