#include "Element.hpp"

namespace rat {
    Element::Element(const std::string& file, float radius, sf::Color color) :
    _radius(radius),
    _isReady(false),
    _background(0.f) {
        _texture.loadFromFile(file);
        _icon.setTexture(_texture);
        float k = (radius*1.5f) / _texture.getSize().y;
        _icon.scale(k,k);

        _background.setOrigin(radius, radius);
        _background.setFillColor(color);
        _icon.setOrigin(_texture.getSize().x/2, _texture.getSize().y/2);
    }

    void Element::update(float deltaTime) {
        if(!_isReady) {
            _background.setRadius(_background.getRadius() + (155.f*deltaTime));
            if(_background.getRadius() >= _radius) {
                _background.setRadius(_radius);
                _isReady = true;
            }
            _background.setOrigin(_background.getRadius(), _background.getRadius());
        }
    }

    void Element::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        states.transform *= getTransform();

        target.draw(_background, states);
        if(_isReady)
            target.draw(_icon, states);
    }
}