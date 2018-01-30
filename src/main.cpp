#include <SFML/System/Err.hpp>

#include "Szczur/Debug/OnDebug.hpp"
#include "Szczur/Application.hpp"

int main()
{
	// Debug stuff
	INIT_LOGGER();
	sf::err().rdbuf(nullptr);

	rat::Application app;
	return app.run();
}
