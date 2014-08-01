#ifndef _SVM_EXCEPTION_H_
#define _SVM_EXCEPTION_H_

#include <sstream>

#include "SVM_Types.h"

#define SVM_EXCEPTION_AUTO(classname, parent) class classname : public parent { \
	public: \
		classname() {} \
		classname(const String& message) : parent(message) {} \
		classname(const String& message, Exception& inner) : parent(message, inner) {} \
	}

namespace Shogun
{
	class Exception
	{
	public:
		Exception()
		{
		}

		Exception(const String& message)
		{
			this->message = message;
		}

		Exception(const String& message, Exception& inner)
		{
			this->message = message;
			this->inner = &inner;
		}

		virtual const String& getMessage() const
		{
			return this->message;
		}

		virtual Exception* getInner() const
		{
			return this->inner;
		}

		virtual String getStackTrace() const
		{
			std::stringstream ss;
			ss << this->getMessage() << std::endl;
			Exception* inner = this->getInner();
			if (inner != 0)
			{
				ss << inner->getStackTrace();
			}

			return ss.str();
		}

	protected:
		String message;

		Exception* inner = 0;
	};

	SVM_EXCEPTION_AUTO(ObjectException, Exception);

	SVM_EXCEPTION_AUTO(ObjectTypeException, ObjectException);

	SVM_EXCEPTION_AUTO(ObjectConversionException, ObjectTypeException);

	SVM_EXCEPTION_AUTO(ObjectReadException, ObjectException);

	SVM_EXCEPTION_AUTO(VMException, ObjectException);

	SVM_EXCEPTION_AUTO(NullPointerException, VMException);

	SVM_EXCEPTION_AUTO(EmptyStackException, NullPointerException);

	SVM_EXCEPTION_AUTO(MemoryException, VMException);

	SVM_EXCEPTION_AUTO(AddressOutOfBoundsException, MemoryException);

	SVM_EXCEPTION_AUTO(InvalidOperationException, VMException);
}

#endif