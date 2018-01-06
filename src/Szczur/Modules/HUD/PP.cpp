#include "PP.hpp"

namespace rat {
    PP::PP(float radius, float gap) :
    _radius(radius),
    _gap(gap) {

    }

    void PP::create(float radius, float gap) {
        _radius = radius;
        _gap = gap;
    }

    PP::~PP() {
        for(auto it : _elements)
            delete it;
        for(auto it : _trash)
            delete it;
    }

    Element* PP::add(size_t id) {
        Element* obj = _createElement(id);
        if(obj) {
             obj->setPosition(
                _radius + ( _radius*2 + _gap) * _elements.size(),
                _radius
            );
            _elements.push_back(obj);
            return obj;
        }
        return nullptr;
    }

    void PP::remove() {
        if(size_t size = _elements.size(); size) {
            
            _trash.push_back( _elements[size-1u]->kill() );
            _elements.pop_back();
        }
    }

    void PP::update(float deltaTime) {
        for(auto it : _elements)
            it->update(deltaTime);
        for(int i = 0; i<_trash.size(); ++i) {
            _trash[i]->update(deltaTime);
            if(_trash[i]->isDead()) {
                delete _trash[i];
                _trash.erase(_trash.begin() + i);
            }

        }
        
    }


    Element* PP::_createElement(size_t id) const {
        switch(id) {
            case ElementType::fire :
                return new Element("data/fire.png", _radius, sf::Color(163,62,62));
            break;

            case ElementType::water:
                return new Element("data/water.png", _radius, sf::Color(58, 146, 168));
            break;

            case ElementType::wind:
                return new Element("data/wind.png", _radius, sf::Color(24, 68, 64));
            break;

            case ElementType::electric:
                return new Element("data/electric.png", _radius, sf::Color(68, 42, 104));
            break;

            default:
                return nullptr;
            break;
        }
    }

    void PP::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        states.transform *= getTransform();
        for(auto it : _elements)
            target.draw(*it, states);
        for(auto it : _trash)
            target.draw(*it, states);
    }
}