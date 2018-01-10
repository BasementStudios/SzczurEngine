#include "Szczur/Core/Application.hpp"
#include "Szczur/Debug/OnDebug.hpp"

int main()
{
	sf::err().rdbuf(nullptr);

	ON_DEBUG(rat::DebugLogger logger; rat::logger = &logger;)

	rat::Application app;
	return app.run();
}
