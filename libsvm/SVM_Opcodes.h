#ifndef _SVM_OPCODES_H_
#define _SVM_OPCODES_H_

#include "SVM_Object.h"
#include "SVM_Types.h"
#include "SVM_Memory.h"


namespace Shogun
{
	class VirtualMachine;

	enum Opcode : UInt32
	{
		NOOP = 0,
		PUSH,
		POP,
		PMMX,
		PPRI,
		SMMX,
		SPRI,
		HALT
	};

	void executeOperation(VirtualMachine* vm, Opcode opcode);
}

#endif