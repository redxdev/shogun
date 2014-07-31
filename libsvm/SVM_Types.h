#ifndef _SVM_TYPES_H_
#define _SVM_TYPES_H_

#include <stdint.h>
#include <string>

namespace Shogun
{
	typedef int8_t Int8;
	typedef int16_t Int16;
	typedef int32_t Int32;
	typedef int64_t Int64;
	typedef uint8_t UInt8;
	typedef uint16_t UInt16;
	typedef uint32_t UInt32;
	typedef uint64_t UInt64;

	typedef bool Bool;
	typedef UInt8 WritableBool;

	typedef float Number;

	typedef std::string String;
}

#endif