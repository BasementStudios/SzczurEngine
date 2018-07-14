#include "Tests.hpp"

#include <vector>
#include <string>
#include <stdexcept>

#include "Szczur/Config.hpp"
#include "Szczur/Utility/Logger.hpp"

// Test macros
#define TEST(GROUP, NAME, CASE) \
	bool testfn_##GROUP##_##NAME##_##CASE(); 									\
	Test test_##GROUP##_##NAME##_##CASE(testfn_##GROUP##_##NAME##_##CASE, 		\
		__FILENAME__, __LINE__, #GROUP, #NAME, #CASE);							\
	inline bool testfn_##GROUP##_##NAME##_##CASE()

// Test helpers
struct Test;
std::vector<Test*> tests;
typedef bool (*TestFunction)();
struct Test {
	TestFunction test_function;
	const char* test_file = nullptr;
	int test_line;
	const char* test_group = nullptr;
	const char* test_name = nullptr;
	const char* test_case = nullptr;

	Test(
		TestFunction _function, 
		const char* _file, 
		int _line,
		const char* _group, 
		const char* _name, 
		const char* _case
	)
	:	test_function(_function),
		test_file(_file),
		test_line(_line),
		test_group(_group),
		test_name(_name),
		test_case(_case)
	{
		tests.push_back(this);
	}

	void operator()()
	{
#define LOG_TEST(...) { rat::logger->log(test_file, test_line, "TEST", __VA_ARGS__); }
		try {
			LOG_TEST("Testing case ", test_case, " in ", test_name, " from ", test_group, "...");
			if (test_function()) {
				LOG_TEST("Test case ", test_case, " in ", test_name, " from ", test_group, " done successful.");
			}
			else {
				LOG_TEST("Test case ", test_case, " in ", test_name, " from ", test_group, " failed!");
			}
		}
		catch (const std::exception& exception) {
			LOG_TEST("Test case ", test_case, " in ", test_name, " from ", test_group, " failed!");
			LOG_EXCEPTION(exception);
		}
#undef LOG_TEST
	}
};



// Reginster files with test here
#include "Szczur/Utility/SFML3D/Tests/RenderLayer.hpp"



// Main testing function
void runTests()
{
	LOG_INFO("Starting tests...");
	for (Test* test : tests) {
		(*test)();
	}
	LOG_INFO("All tests done...");
}
