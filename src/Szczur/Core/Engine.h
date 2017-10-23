#pragma once

#include <SFML/Graphics.hpp>

#include <Szczur/Core/ModulesCollection.h>
#include <Szczur/Core/AssetsLoader.h>
#include <Szczur/Core/Canvas.h>
#include <Szczur/Game/Map/Map.h>

namespace rat {
	class Engine {
	private:

		ModulesCollection<
			AssetsLoader,
			Canvas,
			Map
		> _modules;

		sf::RenderWindow _window;
		sf::Event _event;
		sf::Clock _deltaTimeClock;
		float _deltaTime;

	public:

		Engine() {
			changeResolution(400, 400);
			_modules.get<AssetsLoader>().loadTexturesFromDataDirectories("res_test/data.txt");
			_modules.get<Map>().init();
		}

		void changeResolution(unsigned width, unsigned height) {
			_window.create(sf::VideoMode(width, height), "SzczurEngine v0.0.0");
			_modules.get<Canvas>().resize(width, height);
		}

		void input() {
			while(_window.pollEvent(_event)) {
				if (_event.type == sf::Event::Closed) {
					_window.close();
				}
				if (_event.type == sf::Event::KeyPressed && _event.key.code == sf::Keyboard::Escape) {
					_window.close();
				}
			}
		}

		void update() {
			_deltaTime = _deltaTimeClock.restart().asSeconds();
			_modules.get<Map>().update(_deltaTime);
		}

		void render() {
			_window.clear();
			_modules.get<Map>().render();
			_modules.get<Canvas>().render(_window);
			_window.display();
		}

		int run() {
			while(_window.isOpen()) {
				input();
				update();
				render();
			}

			return 0;
		}
	};
}
