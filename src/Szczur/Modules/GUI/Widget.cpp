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

    void Widget::update(float deltaTime) {
        for(auto it : _children)
            it->update(deltaTime);
    }

    void Widget::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        states.transform *= getTransform();
        for(auto it : _children)
            target.draw(*it, states);
    }
}