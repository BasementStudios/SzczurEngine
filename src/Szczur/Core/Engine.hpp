#pragma once

#include <SFML/Graphics.hpp>

#include "Szczur/Modules/Map/Map.hpp"
#include "AssetsLoader.hpp"
#include "Canvas.hpp"

namespace rat {
	class Engine {
	private:

		m2::utility::ModulesHolder<
			AssetsLoader,
			Canvas,
			Map
		> _modules;

		sf::RenderWindow _window;
		sf::Event _event;
		sf::Clock _mainClock;

	public:

		Engine();

		void changeResolution(const sf::Vector2u& size);

		void input();

		void update();

		void render();

		int run();
	};
}
