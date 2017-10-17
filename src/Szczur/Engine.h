#pragma once

#include <SFML/Graphics.hpp>

#include "Canvas.h"
#include "Core.h"
#include "Map.h"

namespace rat {
	class Engine {
	// Members
	private:
		// Independent core parts
		Canvas canvas;
		Core core;
		
		// Game
		Map map;
		
		// Engine memebers
		sf::RenderWindow window;
		sf::Event event;
		sf::Clock deltaTimeClock; 
		float deltaTime;
		
	// Member functions
	public:
		Engine();
		void changeResolution(int width, int height);
		void input();
		void update();
		void render();
		void run();	
	};
}