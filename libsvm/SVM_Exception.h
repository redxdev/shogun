#ifndef _SVM_EXCEPTION_H_
#define _SVM_EXCEPTION_H_

#include <sstream>

#include "SVM_Types.h"


#define SVM_EXCEPTION_START(classname, parent) class classname : public parent { \
	public: \
		classname() {} \
		classname(const String& message) : parent(message) {}

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

		virtual const String& getMessage() const
		{
			return this->message;
		}

	protected:
		String message;
	};
}

#endif