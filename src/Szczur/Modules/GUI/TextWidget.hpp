#pragma once 

#include <string>

#include <SFML/Graphics.hpp>

#include "Widget.hpp"

namespace rat {
    class TextWidget : public Widget {
    public:
        static sf::Text createText(const std::string& text, const sf::Color& color, size_t charSize);

        TextWidget(const sf::Text& text, const std::string& path );

        void setColor(const sf::Color& newColor);

        void add(char letter);
    protected:
        virtual sf::Vector2u _getSize() const override;
        virtual void _draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    private:
        sf::Text _text;
        sf::Font _font;
    };
}