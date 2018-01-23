#include "TextWidget.hpp"

namespace rat {
    TextWidget::TextWidget() :
    Widget(),
    _text() {

    }

    TextWidget::TextWidget(const sf::Text& text, sf::Font* font) :
    Widget(),
    _text(text) {
        _text.setFont(*font);
    }

    sf::Vector2u TextWidget::_getSize() const {
        auto rect = _text.getGlobalBounds();
        return {
            static_cast<unsigned int>(rect.left + rect.width),
            static_cast<unsigned int>(rect.top + rect.height)
        };
    }

    void TextWidget::_draw(sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw(_text, states);
    }

    void TextWidget::setColor(const sf::Color& newColor) {
        _text.setFillColor(newColor);
    }

    void TextWidget::add(char letter) {
        _text.setString( _text.getString() + letter );
    }

    void TextWidget::removeLast() {
        sf::String str = _text.getString();
        if(str.getSize() > 0){
            str.erase( str.getSize()-1, 1 );
            _text.setString(str);
        }
    }

    size_t TextWidget::getLength() {
        return _text.getString().getSize();
    }

    const std::string& TextWidget::getString() const {
        return _text.getString();
    }

    void TextWidget::setString(const std::string& str) {
        //_text.setString(sf::String::fromUtf8(std::begin(str), std::end(str)));
        _text.setString(getUnicodeString(str));
    }

    void TextWidget::setFont(sf::Font* font) {
        _text.setFont(*font);

    }

    void TextWidget::setCharacterSize(unsigned int size) {
        _text.setCharacterSize(size);
    }

    unsigned int TextWidget::getCharacterSize() const {
        return _text.getCharacterSize();
    }

}