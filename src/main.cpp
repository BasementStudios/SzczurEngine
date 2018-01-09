#include "Szczur/Core/Application.hpp"

int main()
{
	sf::err().rdbuf(nullptr);
	
	rat::DebugLogger logger;
	rat::logger = &logger;

	rat::Application app;
	return app.run();
}
