#include "ShogunVM.h"

namespace Shogun
{
	const char* version_string()
	{
		return SVM_VERSION_STR;
	}

	UInt32 version()
	{
		return SVM_VERSION;
	}
}