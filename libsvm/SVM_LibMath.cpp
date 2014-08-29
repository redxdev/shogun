#include "SVM_LibMath.h"

namespace Shogun
{
	namespace Library
	{
		namespace Math
		{
			void register_library(VirtualMachine* vm)
			{
				vm->registerCallable("rand_int", &rand_int);
			}

			void rand_int(VirtualMachine* vm)
			{
				int min = (int)vm->pop()->getNumber();
				int max = (int)vm->pop()->getNumber();

				int result = rand() % max + min;
				vm->push(createObject((Number)result));
			}
		}
	}
}