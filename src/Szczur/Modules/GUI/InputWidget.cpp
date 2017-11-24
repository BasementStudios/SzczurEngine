#include "InputWidget.hpp"

#include <iostream>

namespace rat {
    InputWidget::InputWidget(sf::Text text, const std::string& path, size_t maxLength) :
    _isActive(false),
    _maxLength(maxLength),
    TextWidget(text, path) {
        _foreground.setSize(static_cast<sf::Vector2f>(getSize()));
        _foreground.setFillColor(sf::Color(255,0,255));

        /*
        Widget::setCallback( CallbackType::onHoverIn, [this](Widget*){
            _isActive = true;
        });

        Widget::setCallback( CallbackType::onHoverOut, [this](Widget*){
            _isActive = false;
        });
        */
    }

    void InputWidget::_input(const sf::Event& event) {
        if(_isActive) {
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

    void InputWidget::active() {
        _isActive = true;
    }

    void InputWidget::deactive() {
        _isActive = false;
    }

    void InputWidget::_draw(sf::RenderTarget& target, sf::RenderStates states) const {
        TextWidget::_draw(target, states);
        if(!_isActive)
            target.draw(_foreground, states);
    }
}