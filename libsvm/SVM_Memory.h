#ifndef _SVM_MEMORY_H_
#define _SVM_MEMORY_H_

#include "SVM_Exception.h"
#include "SVM_Object.h"
#include "SVM_Types.h"

namespace Shogun
{
	class Memory
	{
	public:
		typedef size_t MemSize;

	public:
		Memory(MemSize initialSize = 32);

		~Memory();

		MemSize getSize();

		void resize(MemSize newSize);

		ObjectPtr get(MemSize address);

		void set(MemSize address, ObjectPtr value);

	private:
		ObjectPtr* memory = 0;

		MemSize currentSize = 0;
	};
}

#endif