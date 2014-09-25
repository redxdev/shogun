#include "SVM_VirtualMachine.h"
#include "SVM_Opcodes.h"
#include "ShogunVM.h"

#include <fstream>
#include <iomanip>

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

	VMCallMap& VirtualMachine::getCallMap()
	{
		return this->callMap;
	}

	void VirtualMachine::loadProgram(const Program& program)
	{
		memory.resize(program.size() + 3);
		setRegMmx(program.size() + 2);

		stack.clear();

		UInt32 i = 1;
		for (auto it = program.cbegin(); it != program.cend(); ++it)
		{
			memory.set(i, *it);
			i++;
		}

		setRegPri(1);
	}

	void VirtualMachine::importProgram(const Program& program)
	{
		memory.resize(memory.getSize() + program.size());

		UInt32 i = getRegMmx();
		setRegMmx(memory.getSize());

		for (auto it = program.cbegin(); it != program.cend(); ++it)
		{
			memory.set(i, *it);
			i++;
		}
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

	void VirtualMachine::dump()
	{
		dumpStack(std::cerr);
		
		std::ofstream heap;
		heap.open("svm_heap.dump", std::ios::out | std::ios::trunc);
		if (!heap.is_open()) {
			std::cerr << "Unable to open svm_heap.dump to output heap." << std::endl;
			return;
		}

		dumpHeap(heap);

		std::cerr << "Heap dumped to svm_heap.dump" << std::endl;
	}

	void VirtualMachine::dumpStack(std::ostream& stream)
	{
		auto t = std::time(nullptr);
		auto tm = *std::localtime(&t);

		stream << "ShogunVM version " << Shogun::version_string() << "-" << Shogun::version() << std::endl;
		stream << "dump time - " << std::put_time(&tm, "%Y-%m-%d %H-%M-%S") << std::endl;
		stream << "----------" << std::endl;
		stream << "Registers:" << std::endl;
		stream << "  PRI = " << this->getRegPri() << std::endl;
		stream << "  MMX = " << this->getRegMmx() << std::endl;
		stream << "----------" << std::endl;
		stream << "Stack:" << std::endl;

		for (Stack::iterator it = stack.begin(); it != stack.end(); ++it)
		{
			stream << "> " << (*it)->getReadableString() << std::endl;
		}

		stream << "----------" << std::endl;
		stream << "- end of stack dump" << std::endl;
	}

	void VirtualMachine::dumpHeap(std::ostream& stream)
	{
		auto t = std::time(nullptr);
		auto tm = *std::localtime(&t);

		stream << "ShogunVM version " << Shogun::version_string() << "-" << Shogun::version() << std::endl;
		stream << "dump time - " << std::put_time(&tm, "%Y-%m-%d %H-%M-%S") << std::endl;
		stream << "----------" << std::endl;
		stream << "Registers:" << std::endl;
		stream << "  PRI = " << this->getRegPri() << std::endl;
		stream << "  MMX = " << this->getRegMmx() << std::endl;
		stream << "----------" << std::endl;
		stream << "Heap:" << std::endl;

		for (Memory::MemSize i = 0; i < memory.getSize(); ++i)
		{
			stream << "[" << i << "] " << memory.get(i)->getReadableString() << std::endl;
		}

		stream << "- end of heap dump" << std::endl;
	}
}