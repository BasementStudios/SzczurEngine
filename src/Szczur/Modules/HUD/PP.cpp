#include "PP.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>

namespace rat {
    PP::PP(float radius, float gap) :
    _radius(radius),
    _gap(gap) {
        srand(time(NULL));
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

    void PP::_remove(int index) {
        for(int i = index+1; i<_elements.size(); ++i)
            _elements[i]->moveTargetPosition(sf::Vector2f(-(2*_radius + _gap), 0));

        _trash.push_back( _elements[index]->kill() );
        _elements.erase(_elements.begin() + index);
    }

    void PP::remove() {
        if(size_t size = _elements.size(); size)
            _remove(size-1);
    }

    void PP::randRemove() {
        int size = _elements.size();
        if(size>0) {
            int pos = rand() % size;
            int amount = pos + rand() % (size-pos+1);
            for(int i = pos; i<amount; ++i)
                _remove(pos);
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
            case ElementType::duch :
                return new Element("data/duch.png", _radius, sf::Color(163,62,62, 0));
            break;

            case ElementType::esencja:
                return new Element("data/esencja.png", _radius, sf::Color(58, 146, 168, 0));
            break;

            case ElementType::fizyczny:
                return new Element("data/fizyczny.png", _radius, sf::Color(24, 68, 64, 0));
            break;

            case ElementType::ogien:
                return new Element("data/ogien.png", _radius, sf::Color(68, 42, 104, 0));
            break;

            case ElementType::powietrze:
                return new Element("data/powietrze.png", _radius, sf::Color(68, 42, 104, 0));
            break;

            case ElementType::woda:
                return new Element("data/woda.png", _radius, sf::Color(68, 42, 104, 0));
            break;

            case ElementType::ziemia:
                return new Element("data/ziemia.png", _radius, sf::Color(68, 42, 104, 0));
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