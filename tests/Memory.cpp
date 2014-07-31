#include <gtest/gtest.h>

#include "SVM_Memory.h"
#include "SVM_Object.h"
#include "SVM_Exception.h"
#include "SVM_StringUtils.h"

using namespace Shogun;

namespace Shogun
{
	namespace Tests
	{
		TEST(MemoryTests, MemoryResizing)
		{
			Memory memory(32);

			ASSERT_EQ(32, memory.getSize());
			for (Memory::MemSize i = 0; i < memory.getSize(); i++)
			{
				ObjectPtr object = memory.get(i);
				ASSERT_EQ(Object::NIL, object->getNativeType());
				ASSERT_TRUE(object->isNil());
			}

			ASSERT_THROW(memory.get(32), AddressOutOfBoundsException);

			Memory::MemSize size = rand() % 1000;
			RecordProperty("size", size);
			memory.resize(size);

			ASSERT_EQ(size, memory.getSize());
			for (Memory::MemSize i = 0; i < memory.getSize(); i++)
			{
				ObjectPtr object = memory.get(i);
				ASSERT_EQ(Object::NIL, object->getNativeType());
				ASSERT_TRUE(object->isNil());
			}

			ASSERT_THROW(memory.get(size), AddressOutOfBoundsException);
		}

		TEST(MemoryTests, MemoryAccess)
		{
			Memory::MemSize size = rand() % 1000;
			RecordProperty("size", size);
			Memory memory(size);

			ASSERT_EQ(size, memory.getSize());
			for (Memory::MemSize i = 0; i < memory.getSize(); i++)
			{
				ObjectPtr object = memory.get(i);
				ASSERT_EQ(Object::NIL, object->getNativeType());
				ASSERT_TRUE(object->isNil());
			}

			ASSERT_THROW(memory.get(size), AddressOutOfBoundsException);

			for (Memory::MemSize i = 0; i < memory.getSize(); i++)
			{
				ObjectPtr object = createObject((Number)i);
				memory.set(i, object);
			}

			for (Memory::MemSize i = 0; i < memory.getSize(); i++)
			{
				ObjectPtr object = memory.get(i);
				ASSERT_EQ(i, (Memory::MemSize)object->getNumber());
			}
		}
	}
}