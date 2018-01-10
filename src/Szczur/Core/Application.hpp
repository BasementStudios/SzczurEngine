#pragma once

/** @file Application.cpp
 ** @description Header file with main application class.
 **/

#include <SFML/System/Clock.hpp>
#include <SFML/Window/Window.hpp>

#include "Szczur/Utility/Modules.hpp"
#include "Szczur/Modules/Window/Window.hpp"
#include "Szczur/Modules/Input/Input.hpp"
#include "Szczur/Modules/Assets/Assets.hpp"
#include "Szczur/Modules/Canvas/Canvas.hpp"

namespace rat
{

/** @class Application
 ** @description Main application class.
 **/
class Application
{
	/* Variables */
private:
	// Modules
	ModulesHolder<
		Window,
		Input,
		Assets,
		Canvas
	> _modules;
	
	// Clock
	sf::Clock _mainClock;



	/* Constructor */
public:
	Application();



	/* Methods */
public:
	// Init sequence
	void init();
	
	// Input loop
	void input();

	// Update loop
	void update();

	// Render loop
	void render();

	// Whole runtime
	int run();

	// Module
	template<typename TModule>
	TModule& getModule();
	template<typename TModule>
	const TModule& getModule() const;

	// Window
	sf::Window& getWindow();
	const sf::Window& getWindow() const;
};

}

#include "Application.tpp"
