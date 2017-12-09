#pragma once

#include <SFML/Graphics.hpp>

#include "Widget.hpp"
#include "TextWidget.hpp"

namespace rat {
    class InputWidget : public TextWidget {
    public:
        InputWidget(sf::Text text, sf::Font* font, size_t maxLength);


    protected:
        virtual void _update(float deltaTime) override;
        virtual void _input(const sf::Event& event) override;
        virtual void _draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    private:
        size_t _maxLength;

        sf::RectangleShape _foreground;
    };
}