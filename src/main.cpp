#include "Szczur/Application.hpp"

int main()
{
	// Logger instance on the bottom of the stack to ensure that it will be destructed after all other objects
	rat::Logger ratLogger;

	LOG_INFO("Compiled with " COMPILER_NAME " on " OS_NAME " at " __DATE__ " " __TIME__ " as " MODE_NAME " mode");

	// Actual application code
	rat::Application app;
	return app.run();
}
