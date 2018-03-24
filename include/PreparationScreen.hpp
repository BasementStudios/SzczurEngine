#pragma once

#include <array>
#include <memory>

#include <SFML/Graphics.hpp>

#include "Player.hpp"
#include "Elements.hpp"

namespace rat {
	class PreparationScreen final : public sf::Drawable {
	private:
		const Player& _player;
		const int& _battler;
		const int& _settings;

		std::vector<unsigned short> _elements;
		unsigned int _elements_count;
		
		std::unique_ptr<sf::Sprite> _arrow;
		std::vector<sf::Sprite> _elements_sprites;

	public:
		PreparationScreen(const Player& player, const int& battler, const int& settings);

		void run();
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	};
}