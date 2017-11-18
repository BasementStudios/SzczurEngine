#pragma once

#include <SFML/Graphics.hpp>

#include "Widget.hpp"

namespace rat {
    class CheckboxWidget : public Widget {
    public:
        CheckboxWidget(Widget* parent);
    private:
        bool _isEnabled;

        sf::Sprite _sprite;
        sf::Texture _textureOn;
        sf::Texture _textureOff;

        sf::RectangleShape _tempBackground;

        void _input(sf::Event event) override;
		void _draw(sf::RenderTarget& target, sf::RenderStates states) const override;
		void _update(float deltaTime) override;
    };
}