#include <list>
#include <gtest/gtest.h>
#ifdef _WIN32
#include <windows.h>
#endif

int ARGC;
wchar_t** ARGV;

int main(int argc, char *argv[])
//int wmain(int argc, wchar_t *argv[], wchar_t *envp[])
{
#ifdef _WIN32
	SetProcessDPIAware();
#endif // _WIN32
	//ARGC = argc;
	//ARGV = argv;
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

TEST(Test1, test1) {
    EXPECT_EQ(1, 1);
}
