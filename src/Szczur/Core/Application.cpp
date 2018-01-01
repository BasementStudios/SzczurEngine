#include "Application.hpp"

/** @file Application.cpp
 ** @description Main application class file.
 **/

#include <exception>	// exception

#include <SFML/Graphics.hpp>			// RenderWindow
#include <SFML/System.hpp>				// Clock, Vector2

#include "Szczur/Utility/Logger.hpp"	// LOG_EXCEPTION

namespace rat
{

Application::Application()
{
	_window.create(sf::VideoMode(1280, 720), "SzczurEngine v0.0.0");

	_modules.getModule<Canvas>().init(&_window);
}

void Application::changeResolution(const sf::Vector2u& size)
{
	_window.create(sf::VideoMode(size.x, size.y), "SzczurEngine v0.0.0");

	_modules.getModule<Canvas>().recreateLayers();
}

void Application::input()
{
	sf::Event event;

	while (_window.pollEvent(event)) {
		if (event.type == sf::Event::Closed || _modules.getModule<Input>().isPressed(Keyboard::Escape)) { // delete in final product
			_window.close();
		}
		_modules.forEach<Inputable>([&](auto& mod) {
			mod.input(event);
		});
	}
}

void Application::update()
{
	auto deltaTime = _mainClock.restart().asSeconds();

	_modules.forEach<Updatable>([=](auto& mod) {
		mod.update(deltaTime);
	});

	_modules.getModule<Input>().finish();
}

void Application::render()
{
	_window.clear();

	_modules.forEach<Renderable>([](auto& mod) {
		mod.render();
	});

	_window.display();
}

int Application::run()
{
	// Doubled `while` loop for preventing try-catch block from remarking code with exceptions handler. 
	while (_window.isOpen()) try {
		// Main loop here
		while (_window.isOpen()) {
			input();
			update();
			render();
		}
	} catch (...) {
		LOG_EXCEPTION('\n', boost::current_exception_diagnostic_information());
	}

	return 0;
}

}
