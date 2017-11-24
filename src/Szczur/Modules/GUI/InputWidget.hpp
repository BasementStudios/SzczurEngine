#pragma once

#include <SFML/Graphics.hpp>

#include "Widget.hpp"
#include "TextWidget.hpp"

namespace rat {
    class InputWidget : public TextWidget {
    public:
        InputWidget(sf::Text text, const std::string& path, size_t maxLength);

        void active();
        void deactive();

    protected:
        virtual void _input(const sf::Event& event) override;
        virtual void _draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    private:
        size_t _maxLength;
        bool _isActive;

        sf::RectangleShape _foreground;
    };
}