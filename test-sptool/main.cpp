#include <gmock/gmock.h>

int main(int argc, char** argv)
{
	testing::InitGoogleMock(&argc, argv);
	// testing::GTEST_FLAG(filter) = "-Integration*";
	return RUN_ALL_TESTS();
}