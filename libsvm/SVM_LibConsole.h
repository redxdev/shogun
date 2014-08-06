#ifndef _SVM_LIB_CONSOLE_H_
#define _SVM_LIB_CONSOLE_H_

#include "SVM_VirtualMachine.h"

namespace Shogun
{
	namespace Library
	{
		namespace Console
		{
			void register_library(VirtualMachine* vm);

			void print(VirtualMachine* vm);

			void readline(VirtualMachine* vm);
		}
	}
}

#endif