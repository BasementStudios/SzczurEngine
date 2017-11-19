#pragma once

#include <SFML/Graphics.hpp>

#include <iostream>

#include "Widget.hpp"

namespace rat {
    class ImageWidget : public Widget {
    public:
        ImageWidget(const std::string& fileName);

    private:

        sf::Sprite _sprite;
        sf::Texture _texture;

		void _draw(sf::RenderTarget& target, sf::RenderStates states) const;
		void _update(float deltaTime);
		sf::Vector2u _getSize();
    };
}