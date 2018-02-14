#pragma once

#include "Szczur/Utility/Clock.hpp"
#include "Szczur/Utility/ModulesHolder.hpp"
#include "Szczur/Modules/Assets/Assets.hpp"
#include "Szczur/Modules/Canvas/Canvas.hpp"
#include "Szczur/Modules/Input/Input.hpp"
#include "Szczur/Modules/Window/Window.hpp"

namespace rat
{

class Application
{
public:

	Application() = default;

	Application(const Application&) = delete;
	Application& operator = (const Application&) = delete;

	Application(Application&&) = delete;
	Application& operator = (Application&&) = delete;

	int run();

	void init();

	void input();

	void update();

	void render();

	sf::RenderWindow& getWindow();
	const sf::RenderWindow& getWindow() const;

private:

	ModulesHolder<Window, Canvas, Input, Assets> _modules;
	Clock _mainClock;

};

}
