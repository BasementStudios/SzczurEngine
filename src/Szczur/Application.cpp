#include "Application.hpp"

#include <SFML/Graphics.hpp>

namespace rat
{

void Application::init()
{
	_modules.initModule<Window>();
	_modules.initModule<Input>();
	_modules.initModule<GUI>();
	_modules.initModule<Dialog>("data/dialog.json");
}

void Application::input()
{
	sf::Event event;
	while (getWindow().pollEvent(event)) {
		_modules.getModule<Input>().getManager().processEvent(event);
		_modules.getModule<GUI>().input(event);
		if (event.type == sf::Event::Closed) {
			getWindow().close();
		}
	}
}

void Application::update()
{
	_modules.getModule<Dialog>().update();
	_modules.getModule<GUI>().update();
	_modules.getModule<Input>().getManager().finishLogic();
}

void Application::render()
{
	_modules.getModule<Window>().clear();
	_modules.getModule<GUI>().render();
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
