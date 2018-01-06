#include "Status.hpp"

#include <iostream>

namespace rat {
    Status::Status(float radius, float gap) :
    _radius(radius),
    _gap(gap) {

    }

    Status::~Status() {
        for(auto& it : _elements)
            delete it.second;
        for(auto it : _trash)
            delete it;
    }

    void Status::create(float radius, float gap) {
        _radius = radius;
        _gap = gap;
    }

    Element* Status::add(size_t id) {
        auto it = std::find_if(std::begin(_elements), std::end(_elements), [id](const auto &it){
            return it.first == id;
        });

        if(it == _elements.end()) {
            Element* obj = _createElement(id);
            if(obj) {
                obj->setPosition(
                    _radius + ( _radius*2 + _gap) * _elements.size(),
                    _radius
                );
                _elements.push_back(std::make_pair(id, obj));
                return obj;
            }
        }
        return nullptr;
    }

    void Status::remove(size_t id) {
        auto it = std::find_if(std::begin(_elements), std::end(_elements), [id](const auto &it){
            return it.first == id;
        });
        if(it != _elements.end()) {
            for(auto it2 = it + 1; it2 != _elements.end(); it2++) {
                it2->second->moveTargetPosition(sf::Vector2f(
                        -(2*_radius + _gap),
                        0
                    )
                );
            }
            _trash.push_back(it->second->kill());
            _elements.erase(it);
        }
    }

    void Status::update(float deltaTime) {
        for(auto& it : _elements)
            it.second->update(deltaTime);
        for(int i = 0; i<_trash.size(); ++i) {
            _trash[i]->update(deltaTime);
            if(_trash[i]->isDead()) {
                delete _trash[i];
                _trash.erase(_trash.begin() + i);
                --i;
            }
        }
        
    }

    Element* Status::_createElement(size_t id) const {
        switch(id) {
            case ElementType::fire:
                return new Element("data/fire.png", _radius, sf::Color(160,75,75));
            break;
            case ElementType::water:
                return new Element("data/water.png", _radius, sf::Color(73, 124, 160));
            break;
            case ElementType::wind:
                return new Element("data/wind.png", _radius, sf::Color(24, 68, 64));
            break;
            case ElementType::electric:
                return new Element("data/electric.png", _radius, sf::Color(87, 42, 107));
            break;
            default:
                return nullptr;
            break;
        }
    }

    void Status::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        states.transform *= getTransform();
        for(auto& it : _elements)
            target.draw(*(it.second), states);
        for(auto it : _trash)
            target.draw(*it, states);
    }

}