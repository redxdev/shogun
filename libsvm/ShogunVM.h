#ifndef _SVM_SHOGUN_VM_H_
#define _SVM_SHOGUN_VM_H_

#include "SVM_Types.h"

#define SVM_LIBRARY

#define SVM_VERSION_STR "0.2.0-dev"
#define SVM_VERSION (Shogun::UInt32)20

#define SVM_HEADER_ID "svmo"

namespace Shogun
{
	String version_string();

	UInt32 version();
}

#endif