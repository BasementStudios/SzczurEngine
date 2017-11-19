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
        for(Widget* it : _children){
            switch(event.type) {
                case sf::Event::MouseButtonReleased:
                case sf::Event::MouseButtonPressed:
                case sf::Event::MouseMoved: {
                    sf::Event tempEvent(event);
                    sf::Vector2i itPosition = static_cast<sf::Vector2i>(it->getPosition());
                    sf::Vector2<int*> mousePosition;

                    if(event.type == sf::Event::MouseMoved)
                        mousePosition = { &tempEvent.mouseMove.x, &tempEvent.mouseMove.y };
                    else
                        mousePosition = { &tempEvent.mouseButton.x, &tempEvent.mouseButton.y };

                    //if(*mousePosition.x >= itPosition.x && *mousePosition.y >= itPosition.y) {   
                    *mousePosition.x -= itPosition.x;
                    *mousePosition.y -= itPosition.y;
                    it->input(tempEvent);
                    //}
                    break;
                }

                case sf::Event::TextEntered: {
                    it->input(event);
                    break;
                }

                default: break;
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