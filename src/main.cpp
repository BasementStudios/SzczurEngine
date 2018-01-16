#include <SFML/System/Err.hpp>

#include "Szczur/Debug/OnDebug.hpp"
#include "Szczur/Core/Application.hpp"

int main()
{
	sf::err().rdbuf(nullptr);

	ON_DEBUG(
		rat::DebugLogger mainLogger; rat::logger = &mainLogger;
	);

	rat::Application app;
	return app.run();
}
