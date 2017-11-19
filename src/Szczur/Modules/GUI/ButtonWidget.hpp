#pragma once

#include <SFML/Graphics.hpp>

#include "Widget.hpp"

namespace rat {
    class ButtonWidget : public Widget {
    public:
        ButtonWidget(Widget* parent);
    private:
        bool _isHovered;

        bool _input(const sf::Event& event) override;
		void _draw(sf::RenderTarget& target, sf::RenderStates states) const override;
		void _update(float deltaTime) override;
    };
}

