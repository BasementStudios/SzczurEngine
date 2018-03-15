#pragma once

#include "Szczur/Utility/Time/Clock.hpp"
#include "Szczur/Utility/Modules/Module.hpp"
#include "Szczur/Utility/Modules/ModulesHolder.hpp"
#include "Szczur/Modules/Window/Window.hpp"
#include "Szczur/Modules/Input/Input.hpp"
#include "Szczur/Modules/World/World.hpp"

namespace rat
{

class Application
{
public:

	// Default constructor
	Application() = default;

	// Disable coping
	Application(const Application&) = delete;
	Application& operator = (const Application&) = delete;

	// Disable moving
	Application(Application&&) = delete;
	Application& operator = (Application&&) = delete;

	// Main functions
	int run();

	void init();

	void input();

	void update();

	void render();

	/// Functions to access application Window
	sf::RenderWindow& getWindow();
	const sf::RenderWindow& getWindow() const;

private:

	ModulesHolder<Window, Input, World> _modules;
	Clock _mainClock;

};

}
