#include "Element.hpp"

namespace rat {
    Element::Element(const std::string& file, float radius, sf::Color color) :
    _targetRadius(radius),
    _followRadius(true),
    _followPosition(false),
    _background(0.f) {
        _texture.loadFromFile(file);
        _icon.setTexture(_texture);
        _icon.setScale(0,0);
        _background.setOrigin(radius, radius);
        _background.setFillColor(color);
        _icon.setOrigin(_texture.getSize().x/2, _texture.getSize().y/2);
    }

    void Element::update(float deltaTime) {
        if(_followRadius)
            _followTargetRadius(deltaTime);

        if(_followPosition)
            _followTargetPosition(deltaTime);
    }

    void Element::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        states.transform *= getTransform();

        target.draw(_background, states);
        target.draw(_icon, states);
    }

    void Element::_followTargetRadius(float deltaTime) {
        float curR = _background.getRadius();
        curR = curR + (
            (
                _targetRadius - curR
            )/(5000.f*deltaTime)
        );
        _background.setRadius(curR);
        float k = (curR*1.5f) / _texture.getSize().y;
        _icon.setScale(k,k);
        if(std::abs( curR - _targetRadius ) < 0.1f) {
            _background.setRadius(_targetRadius);
            _followRadius = false;
            _ready = true;
        }
        curR = _background.getRadius();
        _background.setOrigin(curR, curR);
    }

    void Element::_followTargetPosition(float deltaTime) {
        move( 
            (
                _targetPosition - getPosition()
            ) / (10000.f*deltaTime) 
        );
        sf::Vector2f delta = _targetPosition - getPosition();
        delta.x = std::abs(delta.x);
        delta.y = std::abs(delta.y);
        if(delta.x < 0.1f && delta.y < 0.1f) {
            setPosition(_targetPosition);
            _followPosition = false;
        }
    }

    void Element::setTargetRadius(float value) {
        _targetRadius = value;
        _followRadius = true;
    }

    void Element::setTargetPosition(sf::Vector2f value) {
        _targetPosition = value;
        _followPosition = true;
    }
}