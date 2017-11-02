#include "Engine.hpp"

namespace rat {
	Engine::Engine() {
		_modules.get<AssetsLoader>().init();
		_modules.get<AssetsLoader>().loadTexturesFromDataDirectories("res_test/data.txt");

		_modules.get<Canvas>().init();
		_modules.get<Canvas>().addLayer(rat::fnv1a_32("GAME"));
		changeResolution(sf::Vector2u(400, 400));

		_modules.get<Map>().init();
	}

	void Engine::changeResolution(const sf::Vector2u& size) {
		_window.create(sf::VideoMode(size.x, size.y), "SzczurEngine v0.0.0");
		_modules.get<Canvas>().resize(size);
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
		auto _deltaTime = _deltaTimeClock.restart().asSeconds();
		_modules.get<Map>().update(_deltaTime);
	}

	void Engine::render() {
		_window.clear();
		_modules.get<Map>().render();
		_modules.get<Canvas>().display(_window, rat::fnv1a_32("GAME"));
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
