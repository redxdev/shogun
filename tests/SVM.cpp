#include <gtest/gtest.h>

#include <ShogunVM.h>

namespace Shogun
{
	namespace Tests
	{
		TEST(SVM, VersionCheck)
		{
			EXPECT_EQ(SVM_VERSION, version()) << "Library version mismatch. Testing suite likely compiled "
				"a different version of libsvm than what is available.";

			EXPECT_STREQ(SVM_VERSION_STR, version_string()) << "Version string mismatch. Testing suite "
				"likely compiled with a different version of libsvm than what is available.";
		}
	}
}