#include "Engine.h"

namespace rat {
	Engine::Engine() :
		core(canvas), map(core) {
		changeResolution(400, 400);
		map.init();
	}
	void Engine::changeResolution(int width, int height) {
		window.create(sf::VideoMode(width, height), "SzczurEngine v0.0.0");
		canvas.resize(width, height);
	}
	void Engine::input() {
		while(window.pollEvent(event)) {				
			if(event.type == sf::Event::Closed) {
				window.close();
			}
			if((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape)) {
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
		canvas.render(window);
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