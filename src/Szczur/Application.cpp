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

	/// @test Testing Assets & Canvas modules
	tex = &_modules.getModule<Assets>().load<sf::Texture>("null");
	mus = &_modules.getModule<Assets>().load<sf::Music>("null");

	mus->play();
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
	auto deltaTime = _mainClock.restart().asSeconds();

	(void)deltaTime;

	_modules.getModule<Input>().finish();
}

void Application::render()
{
	_modules.getModule<Window>().clear();
	_modules.getModule<Canvas>().render();
	_modules.getModule<Canvas>().render();
	
	/// @test Testing Assets & Canvas modules
	sf::Sprite spr(*tex);
	//_modules.getModule<Canvas>().getLayer<(std::size_t)Layers::First>().draw(spr);
	_modules.getModule<Canvas>().getLayer((std::size_t)Layers::First).draw(spr);
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
