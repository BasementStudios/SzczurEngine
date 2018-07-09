#include "TextWidget.hpp"
#include "Test.hpp"

#include <string>
#include "Szczur/Modules/Script/Script.hpp"
#include "Szczur/Utility/Convert/Unicode.hpp"

#include "Animation/Anim.hpp"
#include "InterfaceWidget.hpp"

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

    sf::Vector2f TextWidget::_getSize() const 
    {
        auto rect = _text.getGlobalBounds();
        return {
            static_cast<float>(rect.width),
            static_cast<float>(rect.height)
        };
    }

    void TextWidget::_draw(sf::RenderTarget& target, sf::RenderStates states) const 
    {
        target.draw(_text, states);
    }

    void TextWidget:: _setColor(const sf::Color& color)
    {
        _text.setFillColor(color);
    }

    void TextWidget::addLetter(char letter) {
        _text.setString( _text.getString() + letter );
        _aboutToRecalculate = true;
    }

    void TextWidget::removeLast() {
        sf::String str = _text.getString();
        if(str.getSize() > 0){
            str.erase( str.getSize()-1, 1 );
            _text.setString(str);
        }
        _aboutToRecalculate = true;
    }

    size_t TextWidget::getLength() {
        return _text.getString().getSize();
    }

    std::string TextWidget::getString() const {
        return _text.getString().toAnsiString();
    }

    void TextWidget::setString(const std::string& str) 
    {
        _text.setString(getUnicodeString(str));
        _aboutToRecalculate = true;
    }

    void TextWidget::setStringInTime(const std::string& str, const gui::AnimData& data)
    {
        if(str == getString()) return;

        using TextAnim_t = gui::Anim<TextWidget, gui::AnimType::Text, std::string>;
        auto setter = static_cast<void (TextWidget::*)(const std::string&)>(&TextWidget::setString);

        auto textAnim = std::make_unique<TextAnim_t>(this, setter);
        textAnim->setAnim(getString(), str, data);
        _addAnimation(std::move(textAnim)); 
    }
    void TextWidget::setStringInTime(const std::string& str, float time)
    {
        setStringInTime(str, gui::AnimData{time});
    }

    void TextWidget::setFont(sf::Font* font) {
        _text.setFont(*font);
        _aboutToRecalculate = true;
    }

    const sf::Font* TextWidget::getFont() const {
        return _text.getFont();
    }

    void TextWidget::setCharacterSize(unsigned int size) {
        _text.setCharacterSize(size);
        _aboutToRecalculate = true;
    }

    unsigned int TextWidget::getCharacterSize() const {
        return _text.getCharacterSize();
    }

    void TextWidget::setCharacterPropSize(float prop)
    {
        _hasChPropSize = true;
        _chPropSize = prop;

        if(_interface) _calcChPropSize();
        else _elementsPropSizeMustBeenCalculated = true;
    }
    float TextWidget::getCharacterPropSize() const
    {
        return _chPropSize;
    }

    void TextWidget::_calcChPropSize()
    {
        assert(_hasChPropSize);
        assert(_interface);

        auto size = _interface->getSizeByPropSize({_chPropSize, _chPropSize});
        setCharacterSize(size_t(std::min(size.x, size.y)));
    }

    void TextWidget::_recalcElementsPropSize()
    {
        if(_hasChPropSize) _calcChPropSize();
    }

    void TextWidget::_callback(CallbackType type) {
        if(auto it = _luaCallbacks.find(type); it != _luaCallbacks.end()) {
            std::invoke(it->second, this);
        }
        if(auto it = _callbacks.find(type); it != _callbacks.end()) {
            std::invoke(it->second, this);
        }
    }

    void TextWidget::_recalcPos()
    {
        _text.setPosition(static_cast<sf::Vector2f>(gui::FamilyTransform::getDrawPosition()));
    }

}