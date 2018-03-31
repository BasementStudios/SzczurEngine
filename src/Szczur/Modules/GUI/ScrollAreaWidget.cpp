#include "ScrollAreaWidget.hpp"
#include <iostream>

#include "Test.hpp"
#include "Szczur/Modules/Script/Script.hpp"

namespace rat {
    ScrollAreaWidget::ScrollAreaWidget() :
    Widget(),
    _scrollSpeed(10.f),
    _offset(0.f) {
        _renderTexture.create(10,10);
    }

    void ScrollAreaWidget::initScript(Script& script) {
        auto object = script.newClass<ScrollAreaWidget>("ScrollAreaWidget", "GUI");

        basicScript(object);
        object.setProperty(
            "size",
            [](ScrollAreaWidget& owner){owner.getSize();},
            [](ScrollAreaWidget& owner, sol::table tab){owner.setSize(tab[1], tab[2]);}
        );
        object.setProperty(
            "scrollSpeed",
            [](ScrollAreaWidget& owner){owner.getScrollSpeed();},
            [](ScrollAreaWidget& owner, float speed){owner.setScrollSpeed(speed);}
        );
        object.init();
    }

    void ScrollAreaWidget::setSize(size_t x, size_t y) {
        _renderTexture.create(x,y);
        _aboutToRecalculate = true;
    }

    void ScrollAreaWidget::setSize(sf::Vector2u size) {
        _renderTexture.create(size.x, size.y);
        _aboutToRecalculate = true;
    }

    sf::Vector2u ScrollAreaWidget::getSize() const {
        return _renderTexture.getSize();
    }

    void ScrollAreaWidget::setScrollSpeed(float speed) {
        _scrollSpeed = speed;
    }

    float ScrollAreaWidget::getScrollSpeed() const {
        return _scrollSpeed;
    }

    sf::Vector2u ScrollAreaWidget::_getSize() const {
        return _renderTexture.getSize();
    }

    void ScrollAreaWidget::calculateSize() {
        _aboutToRecalculate = false;
        _size = _getSize();
        if(_parent != nullptr)
            _parent->calculateSize();
    }

    void ScrollAreaWidget::_draw(sf::RenderTarget& target, sf::RenderStates states) const {
    }


    void ScrollAreaWidget::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        if(isVisible()) {
            states.transform *= getTransform();

            /*sf::RectangleShape shape;
            shape.setSize(static_cast<sf::Vector2f>(getSize()));
            //shape.setFillColor(sf::Color(0,0,255,70));
            shape.setFillColor(sf::Color::Transparent);
            shape.setOutlineColor(sf::Color::White);
            shape.setOutlineThickness(1.f);
            target.draw(shape, states);*/

            _draw(target, states);
            _renderTexture.clear(sf::Color::Transparent);
            for(auto it : _children)
                _renderTexture.draw(*it);

            _renderTexture.display();
            target.draw(sf::Sprite(_renderTexture.getTexture()), states);
        }
    } 

    void ScrollAreaWidget::_update(float deltaTime) {
    }

    void ScrollAreaWidget::_input(const sf::Event& event) {
        if(_isHovered && event.type == sf::Event::MouseWheelScrolled) {
            float offset = _scrollSpeed*static_cast<float>(event.mouseWheelScroll.delta);

            _offset += offset;
            if(_offset > 0) {
                _offset -= offset;
                offset = -_offset;
                _offset = 0;
            }
            for(auto& it : _children)
                it->move({0.f, offset});
        }
    }

    void ScrollAreaWidget::_callback(CallbackType type) {
        if(auto it = _luaCallbacks.find(type); it != _luaCallbacks.end())
            std::invoke(it->second, this);
        if(auto it = _callbacks.find(type); it != _callbacks.end())
            std::invoke(it->second, this);
    }

}