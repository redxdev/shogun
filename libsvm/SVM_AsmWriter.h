#ifndef _SVM_ASM_WRITER_H_
#define _SVM_ASM_WRITER_H_

#include "SVM_Exception.h"
#include "SVM_Parser.h"

#include <iostream>

namespace Shogun
{
	namespace Assembler
	{
		SVM_EXCEPTION_AUTO(AsmBinaryException, Exception);

		SVM_EXCEPTION_AUTO(HeaderIdMismatchException, AsmBinaryException);

		SVM_EXCEPTION_AUTO(VersionMismatchException, AsmBinaryException);

		class AsmWriter
		{
		public:
			AsmWriter();

			void write(std::ostream& stream, NodeList& nodes);

		private:
			void writeHeader(std::ostream& stream, NodeList& nodes);

			void writeNodes(std::ostream& stream, NodeList& nodes);
		};

		class AsmReader
		{
		public:
			AsmReader();

			CompileInfo read(std::istream& stream);

		private:
			void readHeader(std::istream& stream);

			void readObjects(std::istream& stream, CompileInfo& compile);
		};
	}
}

#endif