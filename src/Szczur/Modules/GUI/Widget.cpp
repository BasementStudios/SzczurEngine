#include "Widget.hpp"

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

    sf::Vector2u Widget::getSize() {
        sf::Vector2u size{0u,0u};
        for(Widget* it : _children)
            size += it->getSize();
        return size + _getSize();
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

    


    
}