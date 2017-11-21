#include "Widget.hpp"

#include <iostream>

namespace rat {
    Widget::Widget() :
    _isHovered(false),
    _isPressed(false) {
        ;
    }

    Widget* Widget::setCallback(CallbackType key, Function_t value) {
        _callback.insert_or_assign(key, value);

        return this;
    }

    void Widget::callback(CallbackType type) {
        if(auto it = _callback.find(type); it != _callback.end())
            std::invoke(std::get<1>(*it), this);
    }

    Widget* Widget::add(Widget* object) {
        if(object) 
            _children.push_back(object);
        return object;
    }

    void Widget::input(const sf::Event& event) {
        switch(event.type) {
            case sf::Event::MouseMoved: {
                auto thisSize = getSize();
                if(
                    event.mouseMove.x >= 0 &&
                    event.mouseMove.x <= thisSize.y &&
                    event.mouseMove.y >= 0 &&
                    event.mouseMove.y <= thisSize.y
                ) {
                    if(!_isHovered) {
                        callback(CallbackType::onHoverIn);
                        _isHovered = true;
                    }
                }
                else {
                    if(_isHovered) {
                        callback(CallbackType::onHoverOut);
                        _isHovered = false;
                    }
                }
                break;
            }

            case sf::Event::MouseButtonPressed: {
                if(_isHovered) {
                    callback(CallbackType::onPress);
                    _isPressed = true;
                }
                break;
            }

            case sf::Event::MouseButtonReleased: {
                if(_isPressed) {
                    _isPressed = false;
                    if(_isHovered)
                        callback(CallbackType::onRelease);         
                }
                break;
            }

            default: break;
        }
        for(Widget* it : _children) {
            if(event.type == sf::Event::MouseMoved) {
                auto itPosition = it->getPosition();
                sf::Event tempEvent(event);
                tempEvent.mouseMove.x -= itPosition.x;
                tempEvent.mouseMove.y -= itPosition.y;
                it->input(tempEvent);
            }
            else
                it->input(event);
        }
    }

    void Widget::update(float deltaTime) {
        if(_isHovered) 
            callback(CallbackType::onHover);

        if(_isPressed)
            callback(CallbackType::onHeld);

        for(Widget* it : _children)
            it->update(deltaTime);
    }
    void Widget::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        states.transform *= getTransform();
        sf::RectangleShape shape;
        shape.setFillColor(sf::Color::Yellow);
        shape.setSize({100.f, 100.f});
        target.draw(shape, states);

        for(Widget* it : _children)
            target.draw(*it, states);
    }

    sf::Vector2u Widget::getSize() const {
        return {100u, 100u};
    }
}



/*
namespace rat {
    Widget::Widget() :
        _parent(nullptr) {

    }

    Widget::~Widget() {
        for(auto it : _children)
            delete it;
    }

    void Widget::setParent(Widget* parent) {
        _parent = parent;
    }

    Widget* Widget::add(Widget* widget) {
        if(widget){
            widget->setParent(this);
            _children.push_back(widget);
        }
        return widget;
    }

    sf::Vector2u Widget::getPrecountedSize() {
        return _precountedSize;
    }

    sf::Vector2u Widget::getSize() {
        sf::Vector2u size{0u,0u};
        for(Widget* it : _children) {
            auto itSize = it->getSize();
            auto itPosition = static_cast<sf::Vector2i>(it->getPosition());
            if(itPosition.x + itSize.x > size.x)
                size.x = itPosition.x + itSize.x;
            if(itPosition.y + itSize.y > size.y)
                size.y = itPosition.y + itSize.y;
        }
        auto ownSize = _getSize();
        if(ownSize.x > size.x)
            size.x = ownSize.x;
        if(ownSize.y > size.y)
            size.y = ownSize.y;

        _precountedSize = size;
        return _precountedSize;
    }

    sf::Vector2u Widget::_getSize() {
        return {0u,0u};
    }

//input
    bool Widget::_input(const sf::Event& event) {
        return true;
    }

    void Widget::input(const sf::Event& event) {
        if(_input(event)) {
            for(Widget* it : _children) {
                switch(event.type) {
                    case sf::Event::MouseMoved: {
                        sf::Event tempEvent(event);
                        sf::Vector2i itPosition = static_cast<sf::Vector2i>(it->getPosition());
                        sf::Vector2<int*> mousePosition{ &tempEvent.mouseMove.x, &tempEvent.mouseMove.y };
                        *mousePosition.x -= itPosition.x;
                        *mousePosition.y -= itPosition.y;

                        it->input(tempEvent);
                        break;
                    }

                    case sf::Event::MouseButtonReleased:
                    case sf::Event::MouseButtonPressed:
                    case sf::Event::TextEntered: {
                        it->input(event);
                        break;
                    }

                    default: break;
                }
            }
        }
    }

//update
    void Widget::_update(float deltaTime) {}

    void Widget::update(float deltaTime) {
        for(auto it : _children)
            it->update(deltaTime);
        _update(deltaTime);
    }

//draw
    void Widget::_draw(sf::RenderTarget& target, sf::RenderStates states) const {}

    void Widget::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        states.transform *= getTransform();
        for(auto it : _children)
            target.draw(*it, states);
        _draw(target, states);
    }

    


    
}*/