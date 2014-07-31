#include <gtest/gtest.h>
#include <iostream>

int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	int result = RUN_ALL_TESTS();

#ifdef SVM_TEST_PREVENT_CLOSE
	std::cout << "Press enter to continue..." << std::endl;
	std::getchar();
#endif

	return result;
}