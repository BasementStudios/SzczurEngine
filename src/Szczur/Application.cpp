#include "Application.hpp"

#include <SFML/Graphics.hpp>

namespace rat
{

void Application::init()
{
	_modules.initModule<Window>();
	_modules.initModule<Input>();
}

void Application::input()
{
	sf::Event event;

	while (getWindow().pollEvent(event)) {
		_modules.getModule<Input>().processEvent(event);

		if (event.type == sf::Event::Closed || _modules.getModule<Input>().isPressed(Keyboard::Escape)) { // @warn delete in final product
			getWindow().close();
		}
	}
}

void Application::update()
{
	_modules.getModule<Input>().finishLogic();
}

void Application::render()
{
	_modules.getModule<Window>().clear();
	_modules.getModule<Window>().render();
}

int Application::run()
{
	init();

	while (getWindow().isOpen()) {
		input();
		update();
		render();
	}

	return 0;
}

sf::RenderWindow& Application::getWindow()
{
	return _modules.getModule<Window>().getWindow();
}

const sf::RenderWindow& Application::getWindow() const
{
	return _modules.getModule<Window>().getWindow();
}

}
