#ifndef _SVM_VIRTUAL_MACHINE_H_
#define _SVM_VIRTUAL_MACHINE_H_

#include "SVM_Exception.h"
#include "SVM_StringUtils.h"
#include "SVM_Types.h"
#include "SVM_Object.h"
#include "SVM_Memory.h"

#include <stack>
#include <vector>
#include <unordered_map>

namespace Shogun
{
	SVM_EXCEPTION_AUTO(VMException, ObjectException);

	SVM_EXCEPTION_AUTO(MemoryException, VMException);

	SVM_EXCEPTION_AUTO(AddressOutOfBoundsException, MemoryException);

	SVM_EXCEPTION_AUTO(NullPointerException, VMException);

	SVM_EXCEPTION_AUTO(EmptyStackException, NullPointerException);

	SVM_EXCEPTION_AUTO(InvalidOperationException, VMException);

	SVM_EXCEPTION_AUTO(OperationArgumentException, InvalidOperationException);

	SVM_EXCEPTION_AUTO(VMCallException, VMException);

	SVM_EXCEPTION_AUTO(UnknownVMCallException, VMCallException);

	class VirtualMachine;

	typedef std::stack<ObjectPtr> Stack;

	typedef std::vector<ObjectPtr> Program;

	typedef void(*VMCallable)(VirtualMachine*);

	typedef std::unordered_map<String, VMCallable> VMCallMap;

	class VirtualMachine
	{
	public:
		VirtualMachine(Memory::MemSize initialMemory = 0);

		~VirtualMachine();

		Memory& getMemory();

		Stack& getStack();

		VMCallMap& getCallMap();

		void loadProgram(const Program& program);

		void run();

		inline void push(ObjectPtr value)
		{
			this->getStack().push(value);
		}

		inline ObjectPtr peek()
		{
			if (this->getStack().empty())
				throw EmptyStackException("Tried to peek an empty stack");

			return this->getStack().top();
		}

		inline ObjectPtr pop()
		{
			if (this->getStack().empty())
				throw EmptyStackException("Tried to pop an empty stack");

			ObjectPtr result = this->getStack().top();
			this->getStack().pop();
			return result;
		}

		inline VMCallable getCallable(const String& name)
		{
			auto found = this->getCallMap().find(name);
			if (found == this->getCallMap().end())
				throw new UnknownVMCallException(FORMAT("Unknown VMCallable %s", name.c_str()));

			return found->second;
		}

		inline void call(const String& name)
		{
			this->getCallable(name)(this);
		}

		inline void registerCallable(const String& name, VMCallable callable)
		{
			this->getCallMap()[name] = callable;
		}

		inline UInt32 getRegMmx()
		{
			return this->reg_mmx;
		}

		inline void setRegMmx(UInt32 value)
		{
			this->reg_mmx = value;
		}

		inline UInt32 getRegPri()
		{
			return this->reg_pri;
		}

		inline void setRegPri(UInt32 value)
		{
			this->reg_pri = value;
		}

		inline void halt()
		{
			this->running = false;
		}

	private:
		Memory memory;

		Stack stack;

		VMCallMap callMap;

		UInt32 reg_mmx; // local memory position

		UInt32 reg_pri; // program index

		Bool running = false;
	};
}

#endif