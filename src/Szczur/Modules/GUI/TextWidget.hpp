#pragma once 

#include <string>

#include <SFML/Graphics.hpp>

#include "Szczur/Utility/Unicode.hpp"

#include "Widget.hpp"

namespace rat {
    class TextWidget : public Widget {
    public:
        TextWidget();

        TextWidget(const sf::Text& text, sf::Font* font);

        void setColor(const sf::Color& newColor);

        size_t getLength();

        void add(char letter);

        const std::string& getString() const;
        void setString(const std::string& str);

        void setFont(sf::Font* font);

        void setCharacterSize(unsigned int size);

        unsigned int getCharacterSize() const;

        void removeLast();
    protected:
        virtual sf::Vector2u _getSize() const override;
        virtual void _draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    private:
        sf::Text _text;
    };
}