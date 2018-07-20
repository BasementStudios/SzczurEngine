#include "TextWidget.hpp"

#include <string>
#include "Szczur/Utility/Convert/Unicode.hpp"

#include "Widget-Scripts.hpp"
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
        gui::WidgetScripts::set(object);

        object.set("getLength", &TextWidget::getLength);
        object.set("addLetter", &TextWidget::addLetter);
        object.set("getString", &TextWidget::getString);
        object.set("setString", &TextWidget::setString);
        object.set("setFont", &TextWidget::setFont);
        object.set("setCharacterSize", &TextWidget::setCharacterSize);
        object.set("setCharacterPropSize", &TextWidget::setCharacterPropSize);
        object.set("getCharacterPropSize", &TextWidget::getCharacterPropSize);
        object.set("getCharacterSize", &TextWidget::getCharacterSize);
        object.set("removeLast", &TextWidget::removeLast);
        object.set("setOutlineThickness", &TextWidget::setOutlineThickness);
        object.set("setOutlinePropThickness", &TextWidget::setOutlinePropThickness);
        object.set("setOutlineColor", [](TextWidget& owner, unsigned char r, unsigned char g, unsigned char b, unsigned char a){
            owner.setOutlineColor({r, g, b, a});
        });

        object.init();
    }

    sf::Vector2f TextWidget::_getSize() const 
    {
        auto rect = _text.getGlobalBounds();
        auto drawPos = getDrawPosition();
        return {
            static_cast<float>(rect.width + rect.left - drawPos.x),
            static_cast<float>(rect.height + rect.top - drawPos.y)
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
        
        if(_hasOutlinePropThickness) _calcOutlinePropThickness();
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

    void TextWidget::setOutlineThickness(float thickness)
    {
        assert(thickness >= 0.f);
        _text.setOutlineThickness(thickness);
    }
    void TextWidget::setOutlinePropThickness(float prop)
    {
        _outlinePropThickness = prop;
        _hasOutlinePropThickness = true;

        _calcOutlinePropThickness();
    }
    void TextWidget::setOutlineColor(const sf::Color& color)
    {
        _text.setOutlineColor(color);
    }

    void TextWidget::_calcOutlinePropThickness()
    {
        assert(_hasOutlinePropThickness);
        float chSize = float(_text.getCharacterSize());
        setOutlineThickness(chSize * _outlinePropThickness);
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