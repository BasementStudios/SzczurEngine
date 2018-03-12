#include <iostream>

#include <SFML/System/Err.hpp>

#include "Szczur/Debug.hpp"
#include "Szczur/Application.hpp"


int main()
{
	// TURN OFF too many logs
    std::cerr.rdbuf(nullptr);
	
	INIT_LOGGER();

	rat::Application app;
	return app.run();
}
