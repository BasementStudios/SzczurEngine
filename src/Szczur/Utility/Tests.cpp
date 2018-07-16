#include "Tests.hpp"

#include <vector>
#include <string>
#include <stdexcept>
#include <type_traits>

#include "Szczur/Config.hpp"
#include "Szczur/Utility/Logger.hpp"

namespace testing
{

namespace detail
{

/// ;F
struct TesterInvoker
{
	virtual void operator() () = 0;
};

// Vector with all tests
std::vector<TesterInvoker*> tests;

// Test object registering
void registerTest(TesterInvoker* test)
{
	tests.push_back(test);
}

/// ;F
template <typename TTestClass>
struct Tester : public TesterInvoker
{
	const char* filename;
	int line;
	const char* testName;
	const char* testCase;

	Tester(
		const char* _file, 
		int _line,
		const char* _name, 
		const char* _case
	)
	:	filename(_file),
		line(_line),
		testName(_name),
		testCase(_case)
	{
		registerTest(this);
	}
	
	void operator()()
	{
#define LOG_TEST(...) { rat::logger->log(filename, line, "TEST", __VA_ARGS__); }
		try {
			LOG_TEST("Testing case ", testCase, " in ", testName, "...");
			TTestClass* test = new TTestClass();
			test->SetUp();
			test->Run();
			test->TearDown();
		}
		catch (const std::exception& exception) {
			LOG_TEST("[-] Test case ", testCase, " in ", testName, " failed!");
			LOG_EXCEPTION(exception);
		}
#undef LOG_TEST
	};
};

}

/// Basic fixture test class
struct Test
{
	virtual ~Test() {};
	virtual void SetUp() {};
	virtual void Run() { LOG_INFO("Invaild test?"); }
	virtual void TearDown() {};
};

}

#define TEST(NAME, CASE) \
	struct test_##CLASS##_##CASE : public ::testing::Test {}; 	\
	::testing::detail::Tester<test_##CLASS##_##CASE> tester_##CLASS##_##CASE( \
		__FILENAME__, __LINE__, #NAME, #CASE					\
	);															\
	void test_##CLASS##_##CASE::Run()

#define TEST_F(CLASS, CASE) \
	static_assert(std::is_base_of<::testing::Test, CLASS>::value, "Fixture must derive from ::testing::Test!"); \
	struct test_##CLASS##_##CASE : public CLASS					\
		{ virtual void Run() override; };						\
	::testing::detail::Tester<test_##CLASS##_##CASE> tester_##CLASS##_##CASE( \
		__FILENAME__, __LINE__, #CLASS, #CASE					\
	);															\
	void test_##CLASS##_##CASE::Run()



// Reginster files with test here
#include "Szczur/Utility/SFML3D/Tests/RenderTarget.hpp"
//#include "Szczur/Utility/SFML3D/Tests/RenderLayer.hpp"



// Main testing function
void runTests()
{
	LOG_INFO("Starting tests...");
	for (auto test : ::testing::detail::tests) {
		(*test)();
	}
	LOG_INFO("All tests done...");
}
