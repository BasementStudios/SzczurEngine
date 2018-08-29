#include "Tests.hpp"
#ifdef TESTING

/** @file Tests.cpp
 ** @description Implementation file for testing utilities, based on GoogleTest, modified for this project (visual tests).
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

namespace testing
{

namespace detail
{

std::vector<TesterInvoker*> tests;

}

}



// Register files with test here
//#include "Szczur/Modules/Window/Tests/Window.hpp"
#include "Szczur/Utility/SFML3D/Tests/RenderTarget.hpp"
//#include "Szczur/Utility/SFML3D/Tests/RenderLayer.hpp"
//#include "Szczur/Utility/SFML3D/Tests/Other/Test001.hpp"



// Main testing function
void runTests()
{
	LOG_INFO("Starting tests...");
	for (auto test : ::testing::detail::tests) {
		(*test)();
	}
	LOG_INFO("All tests done...");
}



#endif


