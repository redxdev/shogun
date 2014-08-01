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
			SVM_PRO_OP(Opcode::PUSH);
			SVM_PRO_ARG(500.f);
			SVM_PRO_OP(Opcode::HALT);
			SVM_PROGRAM_END;

			vm.loadProgram(program);

			ASSERT_EQ(6, vm.getMemory().getSize());
			ASSERT_EQ(Opcode::PUSH, vm.getMemory().get(1)->getAddress());
			ASSERT_EQ(500.f, vm.getMemory().get(2)->getNumber());
			ASSERT_EQ(Opcode::HALT, vm.getMemory().get(3)->getAddress());

			vm.run();

			ASSERT_EQ(500.f, vm.peek()->getNumber());
		}
	}
}