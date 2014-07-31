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
		String string_format(const String fmt_str, ...) {
			int final_n, n = ((int)fmt_str.size()) * 2; /* reserve 2 times as much as the length of the fmt_str */
			String str;
			std::unique_ptr<char[]> formatted;
			va_list ap;
			while (1) {
				formatted.reset(new char[n]); /* wrap the plain char array into the unique_ptr */
				strcpy(&formatted[0], fmt_str.c_str());
				va_start(ap, fmt_str);
				final_n = vsnprintf(&formatted[0], n, fmt_str.c_str(), ap);
				va_end(ap);
				if (final_n < 0 || final_n >= n)
					n += abs(final_n - n + 1);
				else
					break;
			}
			return String(formatted.get());
		}
	}
}

#endif