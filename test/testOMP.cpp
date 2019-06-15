#include <gtest/gtest.h>
#include <openMPHelloWorld.h>

TEST(testOMP, helloOMP)
{
	EXPECT_EQ(openMPHelloWorld::OpenMPHello(), 0);
}
