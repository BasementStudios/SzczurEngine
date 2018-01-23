#include "InputWidget.hpp"

#include <iostream>

namespace rat {
    InputWidget::InputWidget() :
    TextWidget(),
    _maxLength(16u),
    _isFocused(false) {
        _background.setSize(sf::Vector2f{_maxLength*getCharacterSize()/1.5f, getCharacterSize()*1.2f});

        setCallback(CallbackType::onHoverIn, [this](Widget* owner){
            _isFocused = true;
        });
        setCallback(CallbackType::onHoverOut, [this](Widget* owner){
            _isFocused = false;
        });
        _aboutToRecalculate = true;
    }

    InputWidget::InputWidget(sf::Text text, sf::Font* font, size_t maxLength) :
    _maxLength(maxLength),
    TextWidget(text, font),
    _isFocused(false) {
        _background.setSize(static_cast<sf::Vector2f>(getSize()));
        _background.setFillColor(sf::Color(255,0,255));
    }

    void InputWidget::_update(float deltaTime) {

    }

    void InputWidget::_input(const sf::Event& event) {
        if(_isFocused) {
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

    sf::Vector2u InputWidget::_getSize() const {
        return static_cast<sf::Vector2u>(_background.getSize());
    }

    void InputWidget::setMaxLength(size_t newValue) {
        _maxLength = newValue;
        _background.setSize(sf::Vector2f{_maxLength*getCharacterSize()/1.5f, getCharacterSize()*1.2f});
        _aboutToRecalculate = true;
    }

    void InputWidget::setBackgroundColor(const sf::Color& color) {
        _background.setFillColor(color);
    }

    void InputWidget::_draw(sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw(_background, states);
        TextWidget::_draw(target, states);
    }
}