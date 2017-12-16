#pragma once

#include <SFML/Graphics.hpp>

#include "Szczur/Modules/Assets/Assets.hpp"
#include "Szczur/Modules/Canvas/Canvas.hpp"
#include "Szczur/Modules/Lua/Lua.hpp"
#include "Szczur/Modules/MusicManager/MusicManager.hpp"

namespace rat {
	class Application {
	private:

		ModulesHolder<
			Assets,
			Lua,
			MusicManager,
			Canvas
		> _modules;

		sf::RenderWindow _window;
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
