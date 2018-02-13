#include <SFML/System/Err.hpp>

#include "Szczur/Debug/OnDebug.hpp"
#include "Szczur/Application.hpp"

int main()
{
	sf::err().rdbuf(nullptr);
	INIT_LOGGER();

	rat::Application app;
	return app.run();
}
