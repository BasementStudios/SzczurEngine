#include "Szczur/Application.hpp"

int main()
{
	// Logger instance on the bottom of the stack to ensure that it will be destructed after all other objects
	rat::Logger ratLogger;
	rat::logger = &ratLogger;

	// Polymorphic debug holder for any object
	#ifdef EDITOR
	rat::GlobalVarsHolder ratGlobalVarsHolder;
	rat::gVar = &ratGlobalVarsHolder;
	#endif

	// Actual application code
	rat::Application app;
	return app.run();
}
