#pragma once

#include <SFML/Graphics.hpp>

#include "Widget.hpp"
#include "TextWidget.hpp"

namespace rat {
    class InputWidget : public Widget {
    public:
        InputWidget(sf::Text text, const std::string& path);
    protected:
        virtual void _input(const sf::Event& event) override;
        virtual void _draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    private:
        TextWidget _textWidget;
    };
}