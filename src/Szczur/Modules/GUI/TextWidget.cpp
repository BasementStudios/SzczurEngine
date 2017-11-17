#include "TextWidget.hpp"

namespace rat {
    TextWidget::TextWidget(Widget* parent, sf::Text text) : 
        Widget(parent),
        _text(text) {
        _font.loadFromFile("data/consolab.ttf");
        _text.setFont(_font);
    }

    void TextWidget::_input(sf::Event event) {
        setPosition( {(float)event.mouseMove.x, (float)event.mouseMove.y} );
        //event.mouseMove.x;
    }

    void TextWidget::_update(float deltaTime) {

    }

    void TextWidget::_draw(sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw(_text, states);
    }
}