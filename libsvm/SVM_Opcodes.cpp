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

			// stack operations //

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

			// register operations //

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

			// heap operations //

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

			// math operations //

			OPCODE(ADD)
			{
				Number a = vm->pop()->getNumber();
				Number b = vm->pop()->getNumber();
				vm->push(createObject(a + b));
			}
			OPCODE_END

			OPCODE(SUB)
			{
				Number a = vm->pop()->getNumber();
				Number b = vm->pop()->getNumber();
				vm->push(createObject(a - b));
			}
			OPCODE_END

			OPCODE(MUL)
			{
				Number a = vm->pop()->getNumber();
				Number b = vm->pop()->getNumber();
				vm->push(createObject(a * b));
			}
			OPCODE_END

			OPCODE(DIV)
			{
				Number a = vm->pop()->getNumber();
				Number b = vm->pop()->getNumber();
				vm->push(createObject(a / b));
			}
			OPCODE_END

			OPCODE(MOD)
			{
				Number a = vm->pop()->getNumber();
				Number b = vm->pop()->getNumber();
				vm->push(createObject(fmod(a, b)));
			}
			OPCODE_END

			OPCODE(AADD)
			{
				UInt32 a = vm->pop()->getAddress();
				UInt32 b = vm->pop()->getAddress();
				vm->push(createObject(a + b));
			}
			OPCODE_END

			OPCODE(ASUB)
			{
				UInt32 a = vm->pop()->getAddress();
				UInt32 b = vm->pop()->getAddress();
				vm->push(createObject(a - b));
			}
			OPCODE_END

			OPCODE(AMUL)
			{
				UInt32 a = vm->pop()->getAddress();
				UInt32 b = vm->pop()->getAddress();
				vm->push(createObject(a * b));
			}
			OPCODE_END

			OPCODE(ADIV)
			{
				UInt32 a = vm->pop()->getAddress();
				UInt32 b = vm->pop()->getAddress();
				vm->push(createObject(a / b));
			}
			OPCODE_END

			OPCODE(AMOD)
			{
				UInt32 a = vm->pop()->getAddress();
				UInt32 b = vm->pop()->getAddress();
				vm->push(createObject(a % b));
			}
			OPCODE_END

			// branch operations //

			OPCODE(JUMP)
			{
				UInt32 a = vm->pop()->getAddress();
				vm->setRegPri(a - 1);
			}
			OPCODE_END

			// flow operations //

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