#pragma once

#include <array>

#include <SFML/Graphics.hpp>

#include <Utility.hpp>

namespace rat {
	struct PreparationScreenData {
		sf::Sprite BackgroundSprite;
		enum class Elements : int {
			Physic,
			Fire,
			Water,
			Ground,
			Size
		};
		std::array<sf::Sprite, enum_v(Elements::Size)> ElementsSprites;
	};

	class PreparationScreen final {
	private:
		const int& _player;
		const int& _battler;

	public:
		PreparationScreen(const int& player, const int& battler);

		bool run();
	};
}