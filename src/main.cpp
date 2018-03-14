#include <SFML/System/Err.hpp>

#include "Szczur/Debug.hpp"
#include "Szczur/Application.hpp"

int main()
{
	INIT_LOGGER();

	rat::Application app;
	return app.run();
}
