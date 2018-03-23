#include "Application.hpp"

#include <SFML/Graphics.hpp>

namespace rat
{

void Application::init()
{
	_modules.initModule<Window>();
	_modules.initModule<Input>();
	_modules.initModule<DragonBones>();
	_modules.initModule<World>();
}

void Application::input()
{
	sf::Event event;

	while (getWindow().pollEvent(event)) {
		_modules.getModule<Input>().getManager().processEvent(event);

		if (event.type == sf::Event::Closed) {
			getWindow().close();
		}
	}
}

void Application::update(float deltaTime)
{
	_modules.getModule<DragonBones>().update(deltaTime);
	
	_modules.getModule<Input>().getManager().finishLogic();
}

void Application::render()
{
	_modules.getModule<Window>().clear();

	// @info Render all things here.
	_modules.getModule<World>().render();

	_modules.getModule<Window>().render();
}

int Application::run()
{
	try {
		// Initialize modules
		init();
		sf::Clock sclock;
		float deltaTime;

		// Main game loop
		while (getWindow().isOpen()) {
			deltaTime = sclock.restart().asSeconds();
			input();
			update(deltaTime);
			render();
		}
	}
	catch (std::exception& exception) {
		LOG_EXCEPTION(exception);
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
