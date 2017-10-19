#include "Engine.h"

#include "Loader.h"

namespace rat {
	Engine::Engine() :
		map(_core) {
		changeResolution(400, 400);
		map.init();
		
		Loader loader;
		loader.loadTexturesFromDataDirectories("res_test/data.txt");
		
	}

	void Engine::changeResolution(unsigned width, unsigned height) {
		window.create(sf::VideoMode(width, height), "SzczurEngine v0.0.0");
		_core.get<Canvas>().resize(width, height);
	}

	void Engine::input() {
		while(window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
				window.close();
			}
		}
	}

	void Engine::update() {
		deltaTime = deltaTimeClock.restart().asSeconds();
		map.update(deltaTime);
	}

	void Engine::render() {
		window.clear();
		map.render();
		_core.get<Canvas>().render(window);
		window.display();
	}

	void Engine::run() {
		while(window.isOpen()) {
			input();
			update();
			render();
		}
	}
}
