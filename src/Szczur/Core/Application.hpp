#pragma once

#include <SFML/Graphics.hpp>

#include "Szczur/Modules/Assets/Assets.hpp"
#include "Szczur/Modules/Canvas/Canvas.hpp"
#include "Szczur/Modules/GUI/GUI.hpp"

namespace rat {
	class Application {
	private:

		ModulesHolder<
			Assets,
			Canvas,
			GUI
		> _modules;

		sf::RenderWindow _window;
		sf::Event _event;
		sf::Clock _mainClock;

	public:

		Application();

		void changeResolution(const sf::Vector2u& size);

		void input();

		void update();

		void render();

		int run();
	};
}
