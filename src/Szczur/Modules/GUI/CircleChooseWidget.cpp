#include "CircleChooseWidget.hpp"
#include <cmath>
#include <iostream>
namespace rat {

    Wedge::Wedge(float radius, float startRotate, float fraction, sf::Vector2f position) :
    _radius(radius),
    _startRotate(startRotate),
    _fraction(fraction),
    _isMoving(false),
    _basePosition(position)
    {
        sf::Shape::update();
        setPosition(position);

        float angle = -3.141592654f/2.f + startRotate + fraction * 3.141592654f;

        _fadeDirection.x = std::cos(angle);
        _fadeDirection.y = std::sin(angle);
        
    }

    void Wedge::update(float deltaTime) {
        if(_isMoving) {
            sf::Vector2f delta = _targetPosition - getPosition();
            float c = std::sqrt(delta.x*delta.x + delta.y*delta.y);
            if(c <= 0.1f) {
                _isMoving = false;
                setPosition(_targetPosition);
            }
            else {
                delta /= c;
                move(delta * deltaTime * c * 20.f);
            }
        }
    }

    bool Wedge::isColliding(sf::Vector2f mouse) {
        sf::Vector2f vertex = _basePosition + sf::Vector2f{_radius, _radius};
        sf::Vector2f delta = mouse - vertex;
        if(delta.x*delta.x + delta.y*delta.y <= _radius*_radius) {
            float angle = 3.141592654f - std::atan2( delta.x, delta.y );
            if(angle >= _startRotate && angle <= _startRotate + 2.f*3.141592654f*_fraction)
                return true;
        }
        return false;
    }

    void Wedge::fadeOut(float distance) {
        _targetPosition = _basePosition + _fadeDirection * distance;
        _isMoving = true;
    }

    void Wedge::fadeIn() {
        _targetPosition = _basePosition;
        _isMoving = true;
    }

    size_t Wedge::getPointCount() const {
        return 10;
    }

    sf::Vector2f Wedge::getPoint(size_t index) const {
        if(index == 0)
            return {_radius, _radius};
        
        float angle = _startRotate + ((2*3.141592654f*_fraction) * (index-1)) / (getPointCount()-2) - (3.141592654f/2.f);

        float x = std::cos(angle) * _radius;
        float y = std::sin(angle) * _radius;
        return sf::Vector2f(_radius + x, _radius + y);
    }
}

namespace rat {
    CircleChooseWidget::CircleChooseWidget()
        : _toRecreate(false), _amount(5u), _radius(10.f), _gap(5.f), _color(255,0,255)
    {}

    CircleChooseWidget::~CircleChooseWidget() {
        for(auto it : _wedges)
            delete it;
    }

    void CircleChooseWidget::setAmount(size_t amount) {
        _amount = amount;
        _toRecreate = true;
    }

    void CircleChooseWidget::setRadius(float radius) {
        _radius = radius;
        _toRecreate = true;
    }

    void CircleChooseWidget::setGap(float gap) {
        _gap = gap;
        _toRecreate = true;
    }

    void CircleChooseWidget::setExtension(float extension) {
        _extension = extension;
        _toRecreate = true;
    }

    void CircleChooseWidget::setColor(sf::Color color) {
        _color = color;
        for(auto it : _wedges) {
            it->setFillColor(color);
        }
    }

    sf::Vector2u CircleChooseWidget::_getSize() const {
        return {
            static_cast<unsigned int>(2*(_radius + _gap + _extension)),
            static_cast<unsigned int>(2*(_radius + _gap + _extension))
        };
    }

    void CircleChooseWidget::_draw(sf::RenderTarget& target, sf::RenderStates states) const {
        for(auto it : _wedges)
            target.draw(*it, states);
    }

    void CircleChooseWidget::_recreate() {
        for(auto it : _wedges)
            delete it;
        _wedges.clear();
        for(size_t i = 0; i < _amount; ++i) {
            float angle = ((2*3.141592654f) * (i)) / (_amount) - (3.141592654f/2.f) + (2.f*3.141592654f / _amount / 2);
            float x = std::cos(angle) * _gap;
            float y = std::sin(angle) * _gap;
            Wedge* wedge = new Wedge(_radius, 2.f*3.141592654f*i/_amount, 1.f/_amount, {x + _gap + _extension, y + _gap + _extension});

            wedge->setFillColor(_color);
            _wedges.push_back(wedge);
        }
    }

    void CircleChooseWidget::_update(float deltaTime) {
        for(auto it : _wedges)
            it->update(deltaTime);
        if(_toRecreate) {
            _recreate();
            _toRecreate = false;
        }
    }

    void CircleChooseWidget::_input(const sf::Event& event) {
        if(event.type == sf::Event::MouseMoved && _isHovered) {
            sf::Vector2f mouse = sf::Vector2f(event.mouseMove.x, event.mouseMove.y);
            for(auto it : _wedges) {
                if(it->isColliding(mouse))
                    it->fadeOut(_extension);
                else
                    it->fadeIn();

            }
        }
    }

}