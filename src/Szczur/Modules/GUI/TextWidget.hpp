#pragma once

#include <SFML/Graphics.hpp>

#include <string>

#include "Widget.hpp"

namespace rat {
    class TextWidget : public Widget {
    public:
        static sf::Text createText(std::string text, unsigned int size, const sf::Color& color);
    public:
        TextWidget(sf::Text text);


    private:
        void _draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        void _update(float deltaTime) override;

        sf::Vector2u _getSize() override;

        sf::Font _font;
        sf::Text _text;
    };
}