#include "Application.hpp"

/** @file Application.cpp
 ** @description Implementation file for application main class.
 **/

#include <exception>

#include <boost/exception/diagnostic_information.hpp> 

#include <SFML/System/Clock.hpp>
#include <SFML/Window/Window.hpp>

#include "Szczur/Utility/Modules.hpp"
#include "Szczur/Debug/Logger.hpp"

namespace rat
{

Application::Application()
{

}

void Application::init()
{
	this->_modules.forEach([&](auto& mod) {
		mod.init();
	});
}

void Application::input()
{
	sf::Event event;

	while (getWindow().pollEvent(event)) {
		_modules.forEach<Module<>::Inputable>([&](auto& mod) {
			mod.input(event);
		});

		 // @warn delete in final product
		if (event.type == sf::Event::Closed || getModule<Input>().isPressed(Keyboard::Escape)) {
			getWindow().close();
		}
	}
}

void Application::update()
{
	auto deltaTime = _mainClock.restart().asSeconds();

	_modules.forEach<Module<>::Updatable>([=](auto& mod) {
		mod.update(deltaTime);
	});
	
	getModule<Input>().finish();
}

void Application::render()
{
	// _modules.forEach<Module<>::Renderable>([](auto& mod) {
	// 	mod.render();
	// });

	getModule<Canvas>().render();
	getModule<Window>().render();
}

int Application::run()
{
	init();

	while (getWindow().isOpen()) {
		try {
			// Main loop here
			while (getWindow().isOpen()) {
				input();
				update();
				render();
			}
		}
		catch (...) {
			LOG_ERROR("Exception occured: \n", boost::current_exception_diagnostic_information());
		}
	}

	return 0;
}

sf::Window& Application::getWindow()
{
	return this->getModule<Window>().getWindow(); 
}
const sf::Window& Application::getWindow() const
{
	return this->getModule<Window>().getWindow();
}

}
