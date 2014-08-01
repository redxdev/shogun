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
			SVM_PRO_PUSH(500.f);
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
			SVM_PRO_PUSH(10.f);
			SVM_PROGRAM_END;

			vm.loadProgram(program);

			ASSERT_THROW(vm.run(), AddressOutOfBoundsException);
		}

		TEST(VirtualMachineTests, MemoryAccess)
		{
			VirtualMachine vm(0);

			SVM_PROGRAM_BEGIN(program);
			SVM_PRO_PUSH("Hello World!");
			SVM_PRO_PUSH(0u);
			SVM_PRO_OP(STLO);
			SVM_PRO_PUSH(0u);
			SVM_PRO_OP(LDLO);
			SVM_PRO_OP(HALT);
			SVM_PROGRAM_END;

			vm.loadProgram(program);
			vm.run();

			ASSERT_EQ("Hello World!", vm.getMemory().get(vm.getRegMmx())->getString());
			ASSERT_EQ("Hello World!", vm.getStack().top()->getString());
		}

		TEST(VirtualMachineTests, NumberMath)
		{
			VirtualMachine vm(0);
			
			SVM_PROGRAM_BEGIN(program);
			SVM_PRO_PUSH(10.f);
			SVM_PRO_PUSH(5.f);
			SVM_PRO_OP(ADD);
			SVM_PRO_PUSH(5.f);
			SVM_PRO_PUSH(10.f);
			SVM_PRO_OP(SUB);
			SVM_PRO_PUSH(10.f);
			SVM_PRO_PUSH(5.f);
			SVM_PRO_OP(MUL);
			SVM_PRO_PUSH(2.f);
			SVM_PRO_PUSH(10.f);
			SVM_PRO_OP(DIV);
			SVM_PRO_PUSH(2.f);
			SVM_PRO_PUSH(5.f);
			SVM_PRO_OP(MOD);
			SVM_PRO_OP(HALT);
			SVM_PROGRAM_END;

			vm.loadProgram(program);
			vm.run();

			ASSERT_FLOAT_EQ(1.f, vm.pop()->getNumber());
			ASSERT_FLOAT_EQ(5.f, vm.pop()->getNumber());
			ASSERT_FLOAT_EQ(50.f, vm.pop()->getNumber());
			ASSERT_FLOAT_EQ(5.f, vm.pop()->getNumber());
			ASSERT_FLOAT_EQ(15.f, vm.pop()->getNumber());
		}

		TEST(VirtualMachineTests, AddressMath)
		{
			VirtualMachine vm(0);

			SVM_PROGRAM_BEGIN(program);
			SVM_PRO_PUSH(10u);
			SVM_PRO_PUSH(5u);
			SVM_PRO_OP(AADD);
			SVM_PRO_PUSH(5u);
			SVM_PRO_PUSH(10u);
			SVM_PRO_OP(ASUB);
			SVM_PRO_PUSH(10u);
			SVM_PRO_PUSH(5u);
			SVM_PRO_OP(AMUL);
			SVM_PRO_PUSH(2u);
			SVM_PRO_PUSH(10u);
			SVM_PRO_OP(ADIV);
			SVM_PRO_PUSH(2u);
			SVM_PRO_PUSH(5u);
			SVM_PRO_OP(AMOD);
			SVM_PRO_OP(HALT);
			SVM_PROGRAM_END;

			vm.loadProgram(program);
			vm.run();

			ASSERT_EQ(1u, vm.pop()->getAddress());
			ASSERT_EQ(5u, vm.pop()->getAddress());
			ASSERT_EQ(50u, vm.pop()->getAddress());
			ASSERT_EQ(5u, vm.pop()->getAddress());
			ASSERT_EQ(15u, vm.pop()->getAddress());
		}

		TEST(VirtualMachineTests, Branching)
		{
			VirtualMachine vm(0);

			SVM_PROGRAM_BEGIN(program);
			SVM_PRO_PUSH(5u);
			SVM_PRO_OP(JUMP);
			SVM_PRO_OP(HALT);
			SVM_PRO_PUSH(100.f);
			SVM_PRO_OP(HALT);
			SVM_PROGRAM_END;

			vm.loadProgram(program);
			vm.run();

			ASSERT_EQ(100.f, vm.pop()->getNumber());
		}
	}
}