#pragma once

/** @file Application.cpp
 ** @description Main application class file.
 **/

#include <SFML/Graphics.hpp>			// RenderWindow
#include <SFML/System.hpp>				// Clock, Vector2

#include "Szczur/Modules/Input/Input.hpp"
#include "Szczur/Modules/Assets/Assets.hpp"
#include "Szczur/Modules/Canvas/Canvas.hpp"
#include "Szczur/Modules/GUI/GUI.hpp"

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
		Input,
		Assets,
		GUI,
		Canvas
	> _modules;

	// Window
	sf::RenderWindow _window;

	// Clock
	sf::Clock _mainClock;



	/* Constructor */
public:
	Application();



	/* Methods */
public:
	// Input loop
	void input();

	// Update loop
	void update();

	// Render loop
	void render();

	// Whole runtime
	int run();

	void changeResolution(const sf::Vector2u& size);
};

}
