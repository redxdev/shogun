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

		// stack operations //
		PUSH, // push a value onto the stack
		POP, // pop a value from the stack (discarding it)
		DUP, // duplicate a value on the stack
		REF, // copy a reference on the stack

		// register operations //
		PMMX, // push the value of the MMX register
		PPRI, // push the value of the PRI register
		SMMX, // store into MMX
		SPRI, // store into PRI

		// heap operations //
		ALLOC, // allocate memory
		DEALLOC, // deallocate memory
		STORE, // store into address
		LOAD, // load address
		STLO, // store into local address
		LDLO, // load local address

		// math operations //
		ADD, // number addition
		SUB, // number subtraction
		MUL, // number multiplication
		DIV, // number division
		MOD, // number mod
		AADD, // same as above, but for addresses
		ASUB,
		AMUL,
		ADIV,
		AMOD,

		// branch operations //
		JUMP,

		// flow operations //
		HALT, // halt execution of the program

		// other //
		COUNT // INVALID
	};

	void executeOperation(VirtualMachine* vm, Opcode opcode);
}

#endif