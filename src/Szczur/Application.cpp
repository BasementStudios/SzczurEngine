#include "Application.hpp"

#include <SFML/Graphics.hpp>

namespace rat
{

void Application::init()
{
	_modules.initModule<Window>();
	_modules.initModule<Canvas>();
	_modules.initModule<Input>();
	_modules.initModule<Assets>();
	_modules.initModule<Script>();
	_modules.initModule<BattleField>();
	
	// std::cout<<'\n'<<std::flush;
	_modules.getModule<BattleField>().testInit();
	std::cout.flush();
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
	auto deltaTime = _mainClock.restart().asFSeconds();

	_modules.getModule<BattleField>().update();
	
	(void)deltaTime;

	_modules.getModule<Input>().finish();
}

void Application::render()
{
	_modules.getModule<Window>().clear();
	_modules.getModule<Canvas>().clear();	
	
	// Tests
	_modules.getModule<Script>().render();
	_modules.getModule<BattleField>().render();
	
	_modules.getModule<Canvas>().render();
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
