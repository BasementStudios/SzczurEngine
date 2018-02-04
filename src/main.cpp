#include <SFML/System/Err.hpp>

#include "Szczur/Debug/OnDebug.hpp"
#include "Szczur/Application.hpp"

int main()
{
	ON_DEBUG(
		sf::err().rdbuf(nullptr);
		INIT_LOGGER();
	);

	rat::Application app;
	return app.run();
}
