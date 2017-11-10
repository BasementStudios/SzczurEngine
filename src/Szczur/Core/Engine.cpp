#include "Engine.hpp"

namespace rat {
	Engine::Engine() {
		_window.create(sf::VideoMode(400, 400), "SzczurEngine v0.0.0");

		_modules.getModule<AssetsLoader>().init();
		_modules.getModule<AssetsLoader>().loadTexturesFromDataDirectories("res_test/data.txt");

		_modules.getModule<Canvas>().init(&_window);

		_modules.getModule<Map>().init();
	}

	void Engine::changeResolution(const sf::Vector2u& size) {
		_window.create(sf::VideoMode(size.x, size.y), "SzczurEngine v0.0.0");
		_modules.getModule<Canvas>().recreateLayers();
	}

	void Engine::input() {
		while(_window.pollEvent(_event)) {
			if (_event.type == sf::Event::Closed) {
				_window.close();
			}
			if (_event.type == sf::Event::KeyPressed && _event.key.code == sf::Keyboard::Escape) {
				_window.close();
			}
		}
	}

	void Engine::update() {
		auto _deltaTime = _mainClock.restart().asSeconds();
		_modules.getModule<Map>().update(_deltaTime);
	}

	void Engine::render() {
		_window.clear();
		_modules.getModule<Map>().render();
		_modules.getModule<Canvas>().display();
		_window.display();
	}

	int Engine::run() {
		while(_window.isOpen()) {
			input();
			update();
			render();
		}

		return 0;
	}
}
