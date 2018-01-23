#pragma once

#include <SFML/Graphics.hpp>

#include "Widget.hpp"
#include "TextWidget.hpp"

namespace rat {
    class InputWidget : public TextWidget {
    public:
        InputWidget();
        InputWidget(sf::Text text, sf::Font* font, size_t maxLength);

        void setMaxLength(size_t newValue);
        void setBackgroundColor(const sf::Color& color);

    protected:
        virtual void _update(float deltaTime) override;
        virtual void _input(const sf::Event& event) override;
        virtual void _draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        virtual sf::Vector2u _getSize() const override;

    private:
        size_t _maxLength;
        bool _isFocused;
        sf::RectangleShape _background;
    };
}