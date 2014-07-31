#ifndef _SVM_STRING_UTILS_H_
#define _SVM_STRING_UTILS_H_

#pragma warning(disable: 4996)

// credit for this goes to http://stackoverflow.com/a/8098080/646180

#include <stdarg.h>  // for va_start, etc
#include <memory>    // for std::unique_ptr
#include <cstring>

#include "SVM_Types.h"

#define FORMAT(fmt, ...) Shogun::Util::string_format(fmt, ##__VA_ARGS__)

namespace Shogun
{
	namespace Util
	{
		String string_format(const String fmt_str, ...);
	}
}

#endif