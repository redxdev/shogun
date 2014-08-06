#include "SVM_LibConsole.h"

#include <iostream>

namespace Shogun
{
	namespace Library
	{
		namespace Console
		{
			void register_library(VirtualMachine* vm)
			{
				vm->registerCallable("print", &print);
				vm->registerCallable("readline", &readline);
			}

			void print(VirtualMachine* vm)
			{
				std::cout << vm->pop()->getString() << std::endl;
			}

			void readline(VirtualMachine* vm)
			{
				String input;
				std::getline(std::cin, input);
				vm->push(createObject(input));
			}
		}
	}
}