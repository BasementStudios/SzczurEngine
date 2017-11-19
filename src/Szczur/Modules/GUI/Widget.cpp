#include "Widget.hpp"

namespace rat {
    Widget::Widget(Widget* parent) :
        _parent(parent) {

    }

    Widget::~Widget() {
        for(auto it : _children)
            delete it;
    }

    Widget* Widget::add(Widget* widget) {
        if(widget)
            _children.push_back(widget);
        return widget;
    }

//input
    void Widget::_input(const sf::Event& event) {}

    void Widget::input(const sf::Event& event) {
        for(auto it : _children) {
            sf::Vector2i mousePosition;
            if(event.type == sf::Event::MouseButtonReleased)
                mousePosition = { event.mouseButton.x, event.mouseButton.y };
            else if(event.type == sf::Event::MouseMoved)
                mousePosition = { event.mouseMove.x, event.mouseMove.y };

            sf::Vector2i itPosition = static_cast<sf::Vector2i>(it->getPosition());
            if(mousePosition.x >= itPosition.x && mousePosition.y >= itPosition.y){
                sf::Event tempEvent(event);

                if(event.type == sf::Event::MouseButtonReleased) {
                    tempEvent.mouseButton.x -= itPosition.x;
                    tempEvent.mouseButton.y -= itPosition.y;
                }

                else if(event.type == sf::Event::MouseMoved) {
                    tempEvent.mouseMove.x -= itPosition.x;
                    tempEvent.mouseMove.y -= itPosition.y;
                }
                
                it->input(tempEvent);
            }
            
        }
        _input(event);
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