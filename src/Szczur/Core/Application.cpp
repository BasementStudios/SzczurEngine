#include "Application.hpp"

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
		if (event.type == sf::Event::Closed) {
			_window.close();
		}
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
			_window.close();
		}
		_modules.forEach([&](auto& mod) {
			mod.input(event);
		});
	}

	
}

void Application::update()
{
	auto deltaTime = _mainClock.restart().asSeconds();

	_modules.forEach([=](auto& mod) {
		mod.update(deltaTime);
	});
}

void Application::render()
{
	_window.clear();

	_modules.forEach([](auto& mod) {
		mod.render();
	});

	_window.display();
}

int Application::run()
{
	while (_window.isOpen()) {
		input();
		update();
		render();
	}

	return 0;
}

}
