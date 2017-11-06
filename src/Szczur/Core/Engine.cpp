#include "Engine.hpp"

namespace rat {
	Engine::Engine() {
		_window.create(sf::VideoMode(400, 400), "SzczurEngine v0.0.0");

		_modules.get_module<AssetsLoader>().init();
		_modules.get_module<AssetsLoader>().loadTexturesFromDataDirectories("res_test/data.txt");

		_modules.get_module<Canvas>().init(&_window);
		_modules.get_module<Canvas>().addLayer(m2::utility::fnv1a_32("GAME"), 0);
		_modules.get_module<Canvas>().addLayer(m2::utility::fnv1a_32("BACK"), 1);

		_modules.get_module<Map>().init();
	}

	void Engine::changeResolution(const sf::Vector2u& size) {
		_window.create(sf::VideoMode(size.x, size.y), "SzczurEngine v0.0.0");
		_modules.get_module<Canvas>().recreateLayers();
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
		_modules.get_module<Map>().update(_deltaTime);
	}

	void Engine::render() {
		_window.clear();
		_modules.get_module<Map>().render();
		_modules.get_module<Canvas>().display();
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
