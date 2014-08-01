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

			// string operations //
			OPCODE(CONCAT)
			{
				String a = vm->pop()->getString();
				String b = vm->pop()->getString();
				vm->push(createObject(a + b));
			}
			OPCODE_END

			// branch operations //

			OPCODE(JUMP)
			{
				UInt32 a = vm->pop()->getAddress();
				vm->setRegPri(a - 1);
			}
			OPCODE_END

			OPCODE(JUMPF)
			{
				Bool a = vm->pop()->getBoolean();
				UInt32 b = vm->pop()->getAddress();
				if (a)
				{
					vm->setRegPri(b - 1);
				}
			}
			OPCODE_END

			// comparison operations //

			OPCODE(CMP)
			{
				ObjectPtr a = vm->pop();
				ObjectPtr b = vm->pop();
				vm->push(createObject(a->equals(b)));
			}
			OPCODE_END

			OPCODE(TCMP)
			{
				ObjectPtr a = vm->pop();
				ObjectPtr b = vm->pop();
				vm->push(createObject(a->tequals(b)));
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

	static StringToOpcodeMap _str_to_opcode;
	static OpcodeToStringMap _opcode_to_str;

#define OPCODE_MAP(opcode) _str_to_opcode[#opcode] = Shogun::Opcode::opcode; \
	_opcode_to_str[Shogun::Opcode::opcode] = #opcode

	void buildOpcodeMaps()
	{
		if (_str_to_opcode.empty() || _opcode_to_str.empty())
		{
			OPCODE_MAP(NOOP);
			OPCODE_MAP(PUSH);
			OPCODE_MAP(POP);
			OPCODE_MAP(DUP);
			OPCODE_MAP(REF);
			OPCODE_MAP(PMMX);
			OPCODE_MAP(PPRI);
			OPCODE_MAP(SMMX);
			OPCODE_MAP(SPRI);
			OPCODE_MAP(ALLOC);
			OPCODE_MAP(DEALLOC);
			OPCODE_MAP(STORE);
			OPCODE_MAP(LOAD);
			OPCODE_MAP(STLO);
			OPCODE_MAP(LDLO);
			OPCODE_MAP(ADD);
			OPCODE_MAP(SUB);
			OPCODE_MAP(MUL);
			OPCODE_MAP(DIV);
			OPCODE_MAP(MOD);
			OPCODE_MAP(AADD);
			OPCODE_MAP(ASUB);
			OPCODE_MAP(AMUL);
			OPCODE_MAP(ADIV);
			OPCODE_MAP(AMOD);
			OPCODE_MAP(CONCAT);
			OPCODE_MAP(JUMP);
			OPCODE_MAP(JUMPF);
			OPCODE_MAP(CMP);
			OPCODE_MAP(TCMP);
			OPCODE_MAP(HALT);
		}
	}

#undef OPCODE_MAP

	Opcode stringToOpcode(const String& str)
	{
		buildOpcodeMaps();
		
		auto found = _str_to_opcode.find(str);
		if (found == _str_to_opcode.end())
			throw InvalidOperationException(FORMAT("Unknown opcode %s", str.c_str()));

		return found->second;
	}

	const String& opcodeToString(Opcode op)
	{
		buildOpcodeMaps();

		auto found = _opcode_to_str.find(op);
		if (found == _opcode_to_str.end())
			throw InvalidOperationException(FORMAT("Unknown opcode %u", (UInt32)op));

		return found->second;
	}
}

#undef OPCODE
#undef OPCODE_END