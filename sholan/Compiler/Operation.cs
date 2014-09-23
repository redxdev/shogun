using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sholan.Compiler
{
    public class Operation
    {
        public Opcode Op
        {
            get;
            set;
        }

        public string Argument
        {
            get;
            set;
        }

        public string Comment
        {
            get;
            set;
        }

        public string Raw
        {
            get;
            set;
        }

        public string GetOutput()
        {
            if (this.Raw != null)
                return this.Raw;

            switch(this.Op)
            {
                case Opcode.LABEL:
                    return string.Format("{0}:{1}", this.Argument, string.IsNullOrEmpty(this.Comment) ? "" : (" ; " + this.Comment));

                case Opcode.NEQ:
                    return string.Format("\tEQ{0}\nNOT", string.IsNullOrEmpty(this.Comment) ? "" : (" ; " + this.Comment));

                case Opcode.NTEQ:
                    return string.Format("\tTEQ{0}\nNOT", string.IsNullOrEmpty(this.Comment) ? "" : (" ; " + this.Comment));

                default:
                    return string.Format("\t{0} {1}{2}", this.Op.ToString(), this.Argument, string.IsNullOrEmpty(this.Comment) ? "" : (" ; " + this.Comment));
            }
        }
    }

    // ported directly from libsvm
    public enum Opcode
    {
        NOOP = 0,

		// stack operations //
		PUSH, // push a value onto the stack
		PUSHNIL,
		PUSHT,
		PUSHF,
		POP, // pop a value from the stack (discarding it)
		DUP, // duplicate a value on the stack
		REF, // copy a reference on the stack
		SWAP, // swap the top two items on the stack

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
		STNLO, // store into local address, negative index
		LDNLO, // load local address, negative index

		// type conversion //
		TBOOL,
		TNUM,
		TADDR,
		TSTR,
		TYPE,

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

		// logic operations //
		AND,
		OR,
		NOT,
		XOR,

		// comparison operations //
		EQ,
		TEQ,
		LT,
		GT,
		LTEQ,
		GTEQ,
		ALT,
		AGT,
		ALTEQ,
		AGTEQ,

		// string operations //
		CONCAT, // concatinate strings

		// branch operations //
		JUMP, // sets pri to a point in memory
		JUMPF, // jump if true

		// comparison operations //
		CMP, // compare a to b regardless of type
		TCMP, // compare a to b, paying attention to type

		// call operations //
		ECALL, // call external function via the call map

		// flow operations //
		HALT, // halt execution of the program
		DBRK, // debug break
		ERR,

		// import operations //
		IMPRT, // import binary

		// assembler-only opcodes //
		GOTO,
		GOTOF,
		PLABL,

		// other //
		COUNT, // INVALID

        LABEL,
        NEQ,
        NTEQ
    }
}
