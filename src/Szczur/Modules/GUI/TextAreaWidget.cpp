#include "TextAreaWidget.hpp"
#include <iostream>
#include "Test.hpp"
namespace rat {
    TextAreaWidget::TextAreaWidget() :
    _size(0u, 0u),
    _toUpdate(false),
    _toCreate(false),
    _toWrap(false) {

    }

    TextAreaWidget::TextAreaWidget(sf::Vector2u size, sf::Font* font) :
    _size(size),
    _toUpdate(false),
    _toCreate(false),
    _toWrap(false) {
        _area.create(size.x * _text.getCharacterSize() * 0.6, size.y * _text.getCharacterSize() * 1.2);
        _view = _area.getView();

        _text.setFont(*font);
        _sprite.setTexture(_area.getTexture());
    }

    void TextAreaWidget::initScript(Script& script) {
        auto object = script.newClass<TextAreaWidget>("TextAreaWidget", "GUI");
        //auto object = script.newClass<ImageWidget>("ImageWidget", "GUI");
        //Widget::basicScript<ImageWidget>(object);
        basicScript(object);

        object.setProperty(
            "font",
            [](TextAreaWidget& owner){owner._text.getFont();},
            [](TextAreaWidget& owner, sf::Font* font){owner.setFont(font);}
        );

        object.setProperty(
            "text",
            [](TextAreaWidget& owner){return owner._text.getString();},
            [](TextAreaWidget& owner, const std::string& text){owner.setString(text);}
        );

        object.setProperty(
            "fontSize",
            [](TextAreaWidget& owner){return owner._text.getCharacterSize();},
            [](TextAreaWidget& owner, size_t size){owner.setCharacterSize(size);}
        );

        object.setProperty(
            "color",
            [](TextAreaWidget& owner){return owner._text.getFillColor();},
            [](TextAreaWidget& owner, sol::table tab){ owner.setColor( sf::Color(tab[1], tab[2], tab[3]) ); }
        );

        object.setProperty(
            "size",
            [](TextAreaWidget& owner){return owner._size;},
            [](TextAreaWidget& owner, sol::table tab){ owner.setSize(sf::Vector2u{tab[1], tab[2]}); }
        );
        
        object.init();
    }

    void TextAreaWidget::setString(const std::string& text) {
        _text.setString(text);
        _toCreate = true;
        _toWrap = true;
    }

    void TextAreaWidget::setSize(sf::Vector2u size) {
        _size = size;
        _toCreate = true;
    }

    void TextAreaWidget::setFont(sf::Font* font) {
        _text.setFont(*font);
        _toCreate = true;
    }

    void TextAreaWidget::setCharacterSize(size_t size) {
        _text.setCharacterSize(size);
        _toCreate = true;
    }

    void TextAreaWidget::setColor(const sf::Color& color) {
        _text.setFillColor(color);
        _toCreate = true;
    }

    void TextAreaWidget::_draw(sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw(_sprite, states);
    }

    void TextAreaWidget::_redrawArea() {
        _area.clear(sf::Color::Transparent);
        _area.draw(_text);
        _area.display();
    }

    void TextAreaWidget::_wrapText() {
        std::string temp = _text.getString().toAnsiString();
        temp.erase(std::remove_if(temp.begin(), temp.end(), [](auto& it){return it=='\n';}), temp.end());
        for(auto i = _size.x; i<temp.size(); i+=_size.x)
            temp.insert(i, 1, '\n');
        _text.setString(temp);
    }

    void TextAreaWidget::_update(float deltaTime) {
        
        if(_toWrap) {
            _toWrap = false;
            _wrapText();
        }
        if(_toCreate) {
            _toCreate = false;
            _area.create(_size.x * _text.getCharacterSize() * 0.6, _size.y * _text.getCharacterSize() * 1.2);
            _redrawArea();
            _view = _area.getView();
            _aboutToRecalculate = true;
            _sprite.setTexture(_area.getTexture());
        }
        if(_toUpdate) {
            _toUpdate = false;
            _area.setView(_view);
            _redrawArea();
        }
        
    }

    void TextAreaWidget::_input(const sf::Event& event) {
        if(_isHovered && event.type == sf::Event::MouseWheelScrolled) {
            _view.move({0.f, -10.f*static_cast<float>(event.mouseWheelScroll.delta)});
            _toUpdate = true;
        }
    }

    sf::Vector2u TextAreaWidget::_getSize() const {
        return _area.getSize();
    }
}