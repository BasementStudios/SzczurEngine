#include "PP.hpp"

namespace rat {
    PP::PP(float radius, float gap) :
    Elements(radius, gap) {

    }

    void PP::create(float radius, float gap) {
        _radius = radius;
        _gap = gap;
    }

    Element* PP::_createElement(size_t id) const {
        switch(id) {
            case ElementType::fire :
                return new Element("data/fire.png", _radius, sf::Color(163,62,62));
            break;

            case ElementType::water:
                return new Element("data/water.png", _radius, sf::Color(58, 146, 168));
            break;

            case ElementType::earth:
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
}