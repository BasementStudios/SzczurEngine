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

		sf::RenderWindow window;
		sf::Event event;
		sf::Clock deltaTimeClock;
		float deltaTime;

	public:

		Engine() {
			changeResolution(400, 400);
			_modules.get<AssetsLoader>().loadTexturesFromDataDirectories("res_test/data.txt");
			_modules.get<Map>().init();
		}

		void changeResolution(unsigned width, unsigned height) {
			window.create(sf::VideoMode(width, height), "SzczurEngine v0.0.0");
			_modules.get<Canvas>().resize(width, height);
		}

		void input() {
			while(window.pollEvent(event)) {
				if (event.type == sf::Event::Closed) {
					window.close();
				}
				if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
					window.close();
				}
			}
		}

		void update() {
			deltaTime = deltaTimeClock.restart().asSeconds();
			_modules.get<Map>().update(deltaTime);
		}

		void render() {
			window.clear();
			_modules.get<Map>().render();
			_modules.get<Canvas>().render(window);
			window.display();
		}

		int run() {
			while(window.isOpen()) {
				input();
				update();
				render();
			}

			return 0;
		}
	};
}
