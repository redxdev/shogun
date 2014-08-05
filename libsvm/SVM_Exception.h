#ifndef _SVM_EXCEPTION_H_
#define _SVM_EXCEPTION_H_

#include <sstream>

#include "SVM_Types.h"


#define SVM_EXCEPTION_START(classname, parent) class classname : public parent { \
	public: \
		classname() {} \
		classname(const String& message) : parent(message) {} \
		classname(const String& message, Exception& inner) : parent(message, inner) {}

#define SVM_EXCEPTION_AUTO(classname, parent) SVM_EXCEPTION_START(classname, parent) SVM_EXCEPTION_END

#define SVM_EXCEPTION_END };

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
}

#endif