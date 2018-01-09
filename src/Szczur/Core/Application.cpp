#include "Application.hpp"

namespace rat
{

Application::Application()
{
	_window.create(sf::VideoMode(1280, 720), "SzczurEngine v0.0.0");
}

void Application::init()
{
	_modules.getModule<Assets>().init();

	_modules.getModule<Canvas>().init(&_window);

	// tex[0] = &_modules.getModule<Assets>().load<sf::Texture>("JenaLee.png");
	// tex[1] = &_modules.getModule<Assets>().load<sf::Texture>("MrRain.png");
	// tex[2] = &_modules.getModule<Assets>().load<sf::Texture>("NotTorvalds.png");
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

	// sf::Sprite spr[3]{ sf::Sprite(*tex[0]), sf::Sprite(*tex[1]), sf::Sprite(*tex[2]) };
	// _window.draw(spr[0]);
	// _window.draw(spr[1]);
	// _window.draw(spr[2]);

	_window.display();
}

int Application::run()
{
	init();

	while (_window.isOpen()) {
		input();
		update();
		render();
	}

	return 0;
}

}
