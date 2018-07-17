#pragma once
#ifdef TESTING

#include <vector>
#include <string>
#include <stdexcept>
#include <type_traits>
#include <chrono>
#include <thread>

#include <SFML/Window/Event.hpp>

#include "Szczur/Config.hpp"
#include "Szczur/Utility/Logger.hpp"
#include "Szczur/Modules/Window/Window.hpp"
#include "Szczur/Utility/Time/Clock.hpp"

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
extern std::vector<TesterInvoker*> tests;

// Test object registering
inline void registerTest(TesterInvoker* test)
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
	
#define LOG_TEST(TEXT) \
	{ 															\
		rat::logger->log(filename, line, "TEST", TEXT);			\
		rat::detail::globalPtr<rat::Window>->getWindow().setTitle(TEXT); \
	}															
	void operator()()
	{
		try {
			LOG_TEST(std::string("Testing case ") + testCase + " in " + testName + "...");
			{
				TTestClass* test = new TTestClass();
				test->SetUp();
				test->Run();
				test->TearDown();
				delete test;
			}
			LOG_TEST(std::string("[+] Test case ") + testCase + " in " + testName + " succeeded!");
		}
		catch (const std::exception& exception) {
			LOG_EXCEPTION(exception);
			LOG_TEST(std::string("[-] Test case ") + testCase + " in " + testName + " failed!");
		}
#undef LOG_TEST
	};
};

}

/// Basic fixture test class
struct Test
{
	virtual void SetUp() {};
	virtual void Run() = 0;
	virtual void TearDown() {};
	virtual ~Test() {};
};

}

#define TEST(NAME, CASE) \
	struct test_##NAME##_##CASE : public ::testing::Test {}; 	\
	::testing::detail::Tester<test_##NAME##_##CASE> tester_##NAME##_##CASE( \
		__FILENAME__, __LINE__, #NAME, #CASE					\
	);															\
	void test_##NAME##_##CASE::Run()

#define TEST_F(CLASS, CASE) \
	static_assert(std::is_base_of<::testing::Test, CLASS>::value, "Fixture must derive from ::testing::Test!"); \
	struct test_##CLASS##_##CASE : public CLASS					\
	{															\
		virtual void Run() override; 							\
	};															\
	::testing::detail::Tester<test_##CLASS##_##CASE> tester_##CLASS##_##CASE( \
		__FILENAME__, __LINE__, #CLASS, #CASE					\
	);															\
	void test_##CLASS##_##CASE::Run()



#define VISUAL_TEST_LOOP(UPDATE, RENDER, INPUT) \
	{													\
		sf::Event event;								\
		rat::Clock clock;								\
		bool testing = true;							\
		sf3d::RenderWindow& window = rat::detail::globalPtr<rat::Window>->getWindow(); \
		while (testing) {								\
			UPDATE;										\
			RENDER;										\
			while (window.pollEvent(event))	{			\
				INPUT;									\
			}											\
		}												\
	}

#define VISUAL_TEST_ANYKEY() \
	VISUAL_TEST_LOOP((void)0;,(void)0;,{				\
		switch (event.type) {							\
			case sf::Event::Closed:						\
			case sf::Event::KeyReleased:				\
				testing = false;						\
				break;									\
			default:									\
				break;									\
		}												\
	})

#define VISUAL_TEST_TIME 3s
#define VISUAL_TEST_WAIT(TIME) \
	using namespace std::chrono_literals; 				\
	std::this_thread::sleep_for(TIME);

#define VISUAL_TEST(NAME, CASE) \
	struct test_##NAME##_##CASE : public NAME					\
	{															\
		virtual ~test_##NAME##_##CASE() 						\
		{														\
			VISUAL_TEST_WAIT(VISUAL_TEST_TIME);					\
		}														\
		virtual void Run() override;							\
	};															\
	::testing::detail::Tester<test_##NAME##_##CASE> tester_##NAME##_##CASE( \
		__FILENAME__, __LINE__, #NAME, #CASE					\
	);															\
	void test_##NAME##_##CASE::Run()

#define VISUAL_TEST_F(CLASS, CASE) \
	static_assert(std::is_base_of<::testing::Test, CLASS>::value, "Fixture must derive from ::testing::Test!"); \
	struct test_##CLASS##_##CASE : public CLASS					\
	{															\
		virtual ~test_##CLASS##_##CASE() 						\
		{														\
			VISUAL_TEST_WAIT(VISUAL_TEST_TIME);					\
		}														\
		virtual void Run() override;							\
	};															\
	::testing::detail::Tester<test_##CLASS##_##CASE> tester_##CLASS##_##CASE( \
		__FILENAME__, __LINE__, #CLASS, #CASE					\
	);															\
	void test_##CLASS##_##CASE::Run()



// Main testing function
void runTests();

#endif // TESTING
