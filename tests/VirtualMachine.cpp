#include <gtest/gtest.h>

#include <list>

#include "SVM_Object.h"
#include "SVM_VirtualMachine.h"
#include "SVM_Exception.h"
#include "SVM_StringUtils.h"
#include "SVM_ProgramUtils.h"

using namespace Shogun;

namespace Shogun
{
	namespace Tests
	{
		TEST(VirtualMachineTests, SimpleProgram)
		{
			VirtualMachine vm(0);

			SVM_PROGRAM_BEGIN(program);
			SVM_PRO_OP(PUSH);
			SVM_PRO_ARG(500.f);
			SVM_PRO_OP(HALT);
			SVM_PROGRAM_END;

			vm.loadProgram(program);

			ASSERT_EQ(6, vm.getMemory().getSize());
			ASSERT_EQ(Opcode::PUSH, vm.getMemory().get(1)->getAddress());
			ASSERT_EQ(500.f, vm.getMemory().get(2)->getNumber());
			ASSERT_EQ(Opcode::HALT, vm.getMemory().get(3)->getAddress());

			vm.run();

			ASSERT_EQ(500.f, vm.peek()->getNumber());
		}

		TEST(VirtualMachineTests, ErrorHandling)
		{
			VirtualMachine vm(0);

			SVM_PROGRAM_BEGIN(program);
			SVM_PRO_OP(POP);
			SVM_PROGRAM_END;

			vm.loadProgram(program);

			ASSERT_THROW(vm.run(), EmptyStackException);

			SVM_PROGRAM_RESET(program); // no halt, should overrun the program bounds and hit
			SVM_PRO_OP(PUSH);			// an address out of bounds exception
			SVM_PRO_ARG(10.f);
			SVM_PROGRAM_END;

			vm.loadProgram(program);

			ASSERT_THROW(vm.run(), AddressOutOfBoundsException);
		}

		TEST(VirtualMachineTests, MemoryAccess)
		{
			VirtualMachine vm(0);

			SVM_PROGRAM_BEGIN(program);
			SVM_PRO_OP(PUSH);
			SVM_PRO_ARG("Hello World!");
			SVM_PRO_OP(PUSH);
			SVM_PRO_ARG(0u);
			SVM_PRO_OP(STLO);
			SVM_PRO_OP(PUSH);
			SVM_PRO_ARG(0u);
			SVM_PRO_OP(LDLO);
			SVM_PRO_OP(HALT);
			SVM_PROGRAM_END;

			vm.loadProgram(program);

			vm.run();

			ASSERT_EQ("Hello World!", vm.getMemory().get(vm.getRegMmx())->getString());
			ASSERT_EQ("Hello World!", vm.getStack().top()->getString());
		}
	}
}