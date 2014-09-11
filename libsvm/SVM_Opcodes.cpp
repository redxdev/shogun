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

			OPCODE(PUSHNIL)
			{
				vm->push(createObject());
			}
			OPCODE_END

			OPCODE(PUSHT)
			{
				vm->push(createObject(true));
			}
			OPCODE_END

			OPCODE(PUSHF)
			{
				vm->push(createObject(false));
			}
			OPCODE_END

			OPCODE(POP)
			{
				vm->pop();
			}
			OPCODE_END

			OPCODE(DUP)
			{
				vm->push(copyObject(vm->peek()));
			}
			OPCODE_END

			OPCODE(REF)
			{
				vm->push(vm->peek());
			}
			OPCODE_END

			OPCODE(SWAP)
			{
				ObjectPtr a = vm->pop();
				ObjectPtr b = vm->pop();
				vm->push(a);
				vm->push(b);
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

			OPCODE(STNLO)
			{
				UInt32 address = vm->pop()->getAddress();
				ObjectPtr data = vm->pop();
				vm->getMemory().set(vm->getRegMmx() - address, data);
			}
			OPCODE_END

			OPCODE(LDNLO)
			{
				UInt32 address = vm->pop()->getAddress();
				vm->push(vm->getMemory().get(vm->getRegMmx() - address));
			}
			OPCODE_END

			// type conversion //

			OPCODE(TBOOL)
			{
				ObjectPtr obj = copyObject(vm->pop());
				obj->setNativeType(Object::BOOLEAN);
				vm->push(obj);
			}
			OPCODE_END;

			OPCODE(TNUM)
			{
				ObjectPtr obj = copyObject(vm->pop());
				obj->setNativeType(Object::NUMBER);
				vm->push(obj);
			}
			OPCODE_END;

			OPCODE(TADDR)
			{
				ObjectPtr obj = copyObject(vm->pop());
				obj->setNativeType(Object::ADDRESS);
				vm->push(obj);
			}
			OPCODE_END;

			OPCODE(TSTR)
			{
				ObjectPtr obj = copyObject(vm->pop());
				obj->setNativeType(Object::STRING);
				vm->push(obj);
			}
			OPCODE_END;

			OPCODE(TYPE)
			{
				vm->push(createObject((UInt32)vm->pop()->getNativeType()));
			}
			OPCODE_END;

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

			// logic operations //

			OPCODE(AND)
			{
				Bool a = vm->pop()->getBoolean();
				Bool b = vm->pop()->getBoolean();
				vm->push(createObject(a && b));
			}
			OPCODE_END

			OPCODE(OR)
			{
				Bool a = vm->pop()->getBoolean();
				Bool b = vm->pop()->getBoolean();
				vm->push(createObject(a || b));
			}
			OPCODE_END

			OPCODE(NOT)
			{
				Bool a = vm->pop()->getBoolean();
				vm->push(createObject(!a));
			}
			OPCODE_END

			OPCODE(XOR)
			{
				Bool a = vm->pop()->getBoolean();
				Bool b = vm->pop()->getBoolean();
				vm->push(createObject(!(a && b) && (a || b)));
			}
			OPCODE_END

			// comparison operations //

			OPCODE(EQ)
			{
				ObjectPtr a = vm->pop();
				ObjectPtr b = vm->pop();
				vm->push(createObject(a->equals(b)));
			}
			OPCODE_END

			OPCODE(TEQ)
			{
				ObjectPtr a = vm->pop();
				ObjectPtr b = vm->pop();
				vm->push(createObject(a->tequals(b)));
			}
			OPCODE_END

			OPCODE(LT)
			{
				Number a = vm->pop()->getNumber();
				Number b = vm->pop()->getNumber();
				vm->push(createObject(a < b));
			}
			OPCODE_END

			OPCODE(GT)
			{
				Number a = vm->pop()->getNumber();
				Number b = vm->pop()->getNumber();
				vm->push(createObject(a > b));
			}
			OPCODE_END

			OPCODE(LTEQ)
			{
					Number a = vm->pop()->getNumber();
					Number b = vm->pop()->getNumber();
					vm->push(createObject(a <= b));
			}
			OPCODE_END

			OPCODE(GTEQ)
			{
					Number a = vm->pop()->getNumber();
					Number b = vm->pop()->getNumber();
					vm->push(createObject(a >= b));
			}
			OPCODE_END

			OPCODE(ALT)
			{
				UInt32 a = vm->pop()->getAddress();
				UInt32 b = vm->pop()->getAddress();
				vm->push(createObject(a < b));
			}
			OPCODE_END

			OPCODE(AGT)
			{
				UInt32 a = vm->pop()->getAddress();
				UInt32 b = vm->pop()->getAddress();
				vm->push(createObject(a > b));
			}
			OPCODE_END

			OPCODE(ALTEQ)
			{
					UInt32 a = vm->pop()->getAddress();
					UInt32 b = vm->pop()->getAddress();
					vm->push(createObject(a <= b));
			}
			OPCODE_END

			OPCODE(AGTEQ)
			{
					UInt32 a = vm->pop()->getAddress();
					UInt32 b = vm->pop()->getAddress();
					vm->push(createObject(a >= b));
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
				UInt32 a = vm->pop()->getAddress();
				Bool b = vm->pop()->getBoolean();
				if (b)
				{
					vm->setRegPri(a - 1);
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

			// call operations //

			OPCODE(ECALL)
			{
				String a = vm->pop()->getString();
				vm->call(a);
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

	StringToOpcodeMap _str_to_opcode;
	OpcodeToStringMap _opcode_to_str;
	OpcodeArgumentMap _opcode_args;

#define OPCODE_MAP(opcode, args) _str_to_opcode[#opcode] = Shogun::Opcode::opcode; \
	_opcode_to_str[Shogun::Opcode::opcode] = #opcode; \
	_opcode_args[Shogun::Opcode::opcode] = args

	void buildOpcodeMaps()
	{
		if (_str_to_opcode.empty() || _opcode_to_str.empty())
		{
			OPCODE_MAP(NOOP, 0);
			OPCODE_MAP(PUSH, 1);
			OPCODE_MAP(PUSHNIL, 0);
			OPCODE_MAP(PUSHT, 0);
			OPCODE_MAP(PUSHF, 0);
			OPCODE_MAP(POP, 0);
			OPCODE_MAP(DUP, 0);
			OPCODE_MAP(REF, 0);
			OPCODE_MAP(SWAP, 0);
			OPCODE_MAP(PMMX, 0);
			OPCODE_MAP(PPRI, 0);
			OPCODE_MAP(SMMX, 0);
			OPCODE_MAP(SPRI, 0);
			OPCODE_MAP(ALLOC, 0);
			OPCODE_MAP(DEALLOC, 0);
			OPCODE_MAP(STORE, 0);
			OPCODE_MAP(LOAD, 0);
			OPCODE_MAP(STLO, 0);
			OPCODE_MAP(LDLO, 0);
			OPCODE_MAP(STNLO, 0);
			OPCODE_MAP(LDNLO, 0);
			OPCODE_MAP(TBOOL, 0);
			OPCODE_MAP(TNUM, 0);
			OPCODE_MAP(TADDR, 0);
			OPCODE_MAP(TSTR, 0);
			OPCODE_MAP(TYPE, 0);
			OPCODE_MAP(ADD, 0);
			OPCODE_MAP(SUB, 0);
			OPCODE_MAP(MUL, 0);
			OPCODE_MAP(DIV, 0);
			OPCODE_MAP(MOD, 0);
			OPCODE_MAP(AADD, 0);
			OPCODE_MAP(ASUB, 0);
			OPCODE_MAP(AMUL, 0);
			OPCODE_MAP(ADIV, 0);
			OPCODE_MAP(AMOD, 0);
			OPCODE_MAP(AND, 0);
			OPCODE_MAP(OR, 0);
			OPCODE_MAP(NOT, 0);
			OPCODE_MAP(XOR, 0);
			OPCODE_MAP(EQ, 0);
			OPCODE_MAP(TEQ, 0);
			OPCODE_MAP(LT, 0);
			OPCODE_MAP(GT, 0);
			OPCODE_MAP(LTEQ, 0);
			OPCODE_MAP(GTEQ, 0);
			OPCODE_MAP(ALT, 0);
			OPCODE_MAP(AGT, 0);
			OPCODE_MAP(ALTEQ, 0);
			OPCODE_MAP(AGTEQ, 0);
			OPCODE_MAP(CONCAT, 0);
			OPCODE_MAP(JUMP, 0);
			OPCODE_MAP(JUMPF, 0);
			OPCODE_MAP(CMP, 0);
			OPCODE_MAP(TCMP, 0);
			OPCODE_MAP(ECALL, 0);
			OPCODE_MAP(GOTO, 1);
			OPCODE_MAP(GOTOF, 1);
			OPCODE_MAP(PLABL, 1);
			OPCODE_MAP(HALT, 0);
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

	UInt8 getOpcodeArgumentCount(Opcode op)
	{
		buildOpcodeMaps();

		auto found = _opcode_args.find(op);
		if (found == _opcode_args.end())
			throw InvalidOperationException(FORMAT("Unknown opcode %u", (UInt32)op));

		return found->second;
	}

	Bool isOpcode(UInt32 op)
	{
		buildOpcodeMaps();

		return _opcode_to_str.find((Opcode)op) != _opcode_to_str.end();
	}
}

#undef OPCODE
#undef OPCODE_END