#pragma once

#include <SFML/Graphics.hpp>

#include <string>

#include "Widget.hpp"

namespace rat {
    class TextWidget : public Widget {
    public:
        static sf::Text createText(std::string text, unsigned int size, sf::Color color) {
            sf::Text temp;
            temp.setString(text);
            temp.setCharacterSize(size);
            temp.setColor(color);
            return temp;
        }
    public:
        TextWidget(Widget* parent, sf::Text text);


    private:
        bool _input(const sf::Event& event) override;
        void _draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        void _update(float deltaTime) override;

        sf::Font _font;
        sf::Text _text;
    };
}