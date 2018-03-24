#pragma once

#include <SFML/Graphics.hpp>

#include "Elements.hpp"
#include <iostream>

namespace rat {
	namespace AssetManager {
		static sf::Color BackgroundColor;
		static std::array<sf::Texture, rat::Elements_Size> ElementsSprites;
		static sf::Texture Arrow;

		static void load_sprites()
		{
			BackgroundColor = { 50, 61, 69 };

			ElementsSprites[0].loadFromFile("res/fire.png");
			ElementsSprites[1].loadFromFile("res/water.png");
			ElementsSprites[2].loadFromFile("res/ground.png");

			Arrow.loadFromFile("res/arrow.png");
		}
	};
}