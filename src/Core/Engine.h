#pragma once

#include <SFML/Graphics.hpp>

#include "Map.h"

namespace rat {
	class Engine {
	private:

		CoreModules _core;

		// TODO Stritch : opakować w GameModules
		Map map;

		sf::RenderWindow window;
		sf::Event event;
		sf::Clock deltaTimeClock;
		float deltaTime;

	public:

		Engine();

		void changeResolution(unsigned width, unsigned height); // TODO Stritch : zwektoryzować

		void input();

		void update();

		void render();

		void run();
	};
}
