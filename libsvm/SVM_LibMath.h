#ifndef _SVM_LIB_MATH_H_
#define _SVM_LIB_MATH_H_

#include "SVM_VirtualMachine.h"

namespace Shogun
{
	namespace Library
	{
		namespace Math
		{
			void register_library(VirtualMachine* vm);

			void rand_int(VirtualMachine* vm);
		}
	}
}

#endif