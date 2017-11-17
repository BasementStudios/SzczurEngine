#include "Application.hpp"

namespace rat {
	Application::Application() {
		_window.create(sf::VideoMode(1280, 720), "SzczurEngine v0.0.0");

		_modules.getModule<Canvas>().init(&_window);
		_modules.getModule<GUI>().init();
	}

	void Application::changeResolution(const sf::Vector2u& size) {
		_window.create(sf::VideoMode(size.x, size.y), "SzczurEngine v0.0.0");

		_modules.getModule<Canvas>().recreateLayers();
	}

	void Application::input() {
		while (_window.pollEvent(_event)) {
			if (_event.type == sf::Event::Closed) {
				_window.close();
			}
			if (_event.type == sf::Event::KeyPressed && _event.key.code == sf::Keyboard::Escape) {
				_window.close();
			}

			_modules.getModule<GUI>().input(_event);

		}
	}

	void Application::update() {
		auto deltaTime = _mainClock.restart().asSeconds();
		_modules.getModule<GUI>().update(deltaTime);
	}

	void Application::render() {
		_window.clear();
		_modules.getModule<GUI>().render();
		_modules.getModule<Canvas>().display();
		_window.display();
	}

	int Application::run() {
		while(_window.isOpen()) {
			input();
			update();
			render();
		}

		return 0;
	}
}
