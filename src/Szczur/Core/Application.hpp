#pragma once

#include <SFML/Graphics.hpp>

#include "Szczur/Modules/Input/Input.hpp"
#include "Szczur/Modules/Assets/Assets.hpp"
#include "Szczur/Modules/Canvas/Canvas.hpp"

namespace rat
{

class Application
{
private:

	ModulesHolder<
		Input,
		Assets,
		Canvas
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
