#include "TextWidget.hpp"

#include <iostream>

namespace rat {
    TextWidget::TextWidget(sf::Text text) : 
        _text(text) {
        _font.loadFromFile("data/consolab.ttf");
        _text.setFont(_font);
    }

    sf::Text TextWidget::createText(std::string text, unsigned int size, const sf::Color& color) {
        sf::Text temp;
        temp.setString(text);
        temp.setCharacterSize(size);
        temp.setFillColor(color);
        return temp;
    }

    void TextWidget::_update(float deltaTime) {
    }

    void TextWidget::_draw(sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw(_text, states);
    }

    sf::Vector2u TextWidget::_getSize() {
        auto it  = _text.getGlobalBounds();
        return { 
            static_cast<unsigned int>( it.left + it.width ),
            static_cast<unsigned int>( it.top + it.height )
        };
    }
}