#include "InputWidget.hpp"

#include <iostream>

namespace rat {
    InputWidget::InputWidget(sf::Text text, sf::Font* font, size_t maxLength) :
    _maxLength(maxLength),
    TextWidget(text, font) {
        deactivate();
        _foreground.setSize(static_cast<sf::Vector2f>(getSize()));
        _foreground.setFillColor(sf::Color(0,0,0,100));
    }

    void InputWidget::_update(float deltaTime) {
        _foreground.setSize(static_cast<sf::Vector2f>(getSize()));
    }

    void InputWidget::_input(const sf::Event& event) {
        if(isActivated()) {
            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode < 128) {
                    if(event.text.unicode == '\b')
                        removeLast();
                    else 
                        if(getLength() < _maxLength)
                            add(static_cast<char>(event.text.unicode));
                    _aboutToRecalculate = true;
                }
            }
        }

    }


    void InputWidget::_draw(sf::RenderTarget& target, sf::RenderStates states) const {
        TextWidget::_draw(target, states);
        if(!isActivated())
            target.draw(_foreground, states);
    }
}