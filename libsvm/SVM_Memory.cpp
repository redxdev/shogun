#include "SVM_Memory.h"
#include "SVM_StringUtils.h"

namespace Shogun
{
	Memory::Memory(MemSize initialSize)
	{
		this->memory = new ObjectPtr[initialSize];
		for (MemSize i = 0; i < initialSize; i++)
		{
			this->memory[i] = createObject();
		}

		this->currentSize = initialSize;
	}

	Memory::~Memory()
	{
		delete[] this->memory;
	}

	Memory::MemSize Memory::getSize()
	{
		return this->currentSize;
	}

	void Memory::resize(MemSize newSize)
	{
		ObjectPtr* newMemory = new ObjectPtr[newSize];
		for (MemSize i = 0; i < (this->currentSize < newSize ? this->currentSize : newSize); i++)
		{
			newMemory[i] = this->memory[i];
		}

		for (MemSize i = this->currentSize; i < newSize; i++)
		{
			newMemory[i] = createObject();
		}

		this->currentSize = newSize;
		this->memory = newMemory;
	}

	ObjectPtr Memory::get(MemSize address)
	{
		if (address >= this->currentSize)
		{
			throw AddressOutOfBoundsException(FORMAT("Address %u is out of bounds (%u allocated)", address, this->currentSize));
		}

		return this->memory[address];
	}

	void Memory::set(MemSize address, ObjectPtr value)
	{
		if (address >= this->currentSize)
		{
			throw AddressOutOfBoundsException(FORMAT("Address %u is out of bounds (%u allocated)", address, this->currentSize));
		}

		this->memory[address] = value;
	}
}