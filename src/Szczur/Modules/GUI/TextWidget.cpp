#include "TextWidget.hpp"
#include "Test.hpp"

#include <string>
#include "Szczur/Modules/Script/Script.hpp"
#include "Szczur/Utility/Convert/Unicode.hpp"
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

    void TextWidget::initScript(Script& script) {
        auto object = script.newClass<TextWidget>("TextWidget", "GUI");
        //auto object = script.newClass<ImageWidget>("ImageWidget", "GUI");
        //Widget::basicScript<ImageWidget>(object);
        basicScript(object);

        object.setProperty(
            "font",
            [](TextWidget& owner){owner.getFont();},
            [](TextWidget& owner, sf::Font* font){owner.setFont(font);}
        );

        object.setProperty(
            "text",
            [](TextWidget& owner){return owner._text.getString();},
            [](TextWidget& owner, const std::string& text){owner.setString(text);}
        );

        object.setProperty(
            "fontSize",
            [](TextWidget& owner){return owner._text.getCharacterSize();},
            [](TextWidget& owner, size_t size){owner.setCharacterSize(size);}
        );

        object.setProperty(
            "color",
            [](TextWidget& owner){return owner._text.getFillColor();},
            [](TextWidget& owner, sol::table tab){ owner.setColor( sf::Color(tab[1], tab[2], tab[3]) ); }
        );
        
        object.init();
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

    void TextWidget::addLetter(char letter) {
        _text.setString( _text.getString() + letter );
        calculateSize();
    }

    void TextWidget::removeLast() {
        sf::String str = _text.getString();
        if(str.getSize() > 0){
            str.erase( str.getSize()-1, 1 );
            _text.setString(str);
        }
        calculateSize();
    }

    size_t TextWidget::getLength() {
        return _text.getString().getSize();
    }

    std::string TextWidget::getString() const {
        return _text.getString().toAnsiString();
    }

    void TextWidget::setString(const std::string& str) {
        //_text.setString(sf::String::fromUtf8(std::begin(str), std::end(str)));
        //_aboutToRecalculate=true;
        _text.setString(getUnicodeString(str));
        calculateSize();
    }

    void TextWidget::setFont(sf::Font* font) {
        _text.setFont(*font);
        calculateSize();
    }

    const sf::Font* TextWidget::getFont() const {
        return _text.getFont();
    }

    void TextWidget::setCharacterSize(unsigned int size) {
        _text.setCharacterSize(size);
        calculateSize();
    }

    unsigned int TextWidget::getCharacterSize() const {
        return _text.getCharacterSize();
    }

    void TextWidget::_callback(CallbackType type) {
        if(auto it = _luaCallbacks.find(type); it != _luaCallbacks.end()) {
            std::invoke(it->second, this);
        }
        if(auto it = _callbacks.find(type); it != _callbacks.end()) {
            std::invoke(it->second, this);
        }
    }

}