#include "SVM_VirtualMachine.h"
#include "SVM_Opcodes.h"

namespace Shogun
{
	VirtualMachine::VirtualMachine(Memory::MemSize initialMemory)
		: memory(initialMemory)
	{
	}

	VirtualMachine::~VirtualMachine()
	{
	}

	Memory& VirtualMachine::getMemory()
	{
		return this->memory;
	}

	Stack& VirtualMachine::getStack()
	{
		return this->stack;
	}

	void VirtualMachine::loadProgram(const Program& program)
	{
		memory.resize(program.size() + 3);
		setRegMmx(program.size() + 2);

		UInt32 i = 1;
		for (auto it = program.cbegin(); it != program.cend(); ++it)
		{
			memory.set(i, *it);
			i++;
		}

		setRegPri(1);
	}

	void VirtualMachine::run()
	{
		this->running = true;
		while (this->running)
		{
			UInt32 opcode = memory.get(getRegPri())->getAddress();
			executeOperation(this, (Opcode)opcode);
			this->setRegPri(this->getRegPri() + 1);
		}
	}
}