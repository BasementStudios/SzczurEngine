#pragma once

#include <SFML/Graphics.hpp>

#include "Szczur/Modules/Canvas/Canvas.hpp"
#include "Szczur/Modules/GUI/GUI.hpp"
#include "Szczur/Modules/Lua/Lua.hpp"
#include "Szczur/Modules/World/World.hpp"

namespace rat
{

class Application
{
private:

	ModulesHolder<
		Canvas,
		GUI,
		Lua,
		World
	> _modules;

	sf::RenderWindow _window;
	sf::Clock _mainClock;

public:

	Application();

	void changeResolution(const sf::Vector2u& size);

	void input();

	void update();

	void render();

	int run();

};

}
