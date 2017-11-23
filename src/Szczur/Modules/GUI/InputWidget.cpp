#include "InputWidget.hpp"

#include <iostream>

namespace rat {
    InputWidget::InputWidget(sf::Text text, const std::string& path) :
    Widget(),
    _textWidget(text, path) {

    }

    void InputWidget::_input(const sf::Event& event) {
        if (event.type == sf::Event::TextEntered) {
            if (event.text.unicode < 128) {
                if(event.text.unicode == '\b')
                    _textWidget.removeLast();
                else 
                    _textWidget.add(static_cast<char>(event.text.unicode));
            }
        }

    }

    void InputWidget::_draw(sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw(_textWidget, states);
    }

    const std::string& InputWidget::getString() const {
        return _textWidget.getString();
    }
}