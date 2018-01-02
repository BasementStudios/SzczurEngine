#include "Elements.hpp"
#include <iostream>
namespace rat {
    Elements::Elements(float radius, float gap) :
    _radius(radius),
    _gap(gap) {

    }

    Elements::~Elements() {
        for(auto& it : _elements)
            delete it;
    }

    Element* Elements::add(size_t id) {
        Element* element = _createElement(id);
        if(element) {
            element->setPosition(
                _radius + (_elements.size() * (_radius * 2 + _gap) ),
                _radius
            );
            _elements.push_back(element); 
        }
        return element;
    }

    void Elements::update(float deltaTime) {
        for(auto& it : _elements)
            it->update(deltaTime);
    }

    void Elements::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        states.transform *= getTransform();
        for(auto& it : _elements)
            target.draw(*it, states);
    }

    Element* Elements::_createElement(size_t id) const {
        return nullptr;
    }
}