#pragma once

#include <vector>

#include <SFML/Graphics.hpp>

#include "Element.hpp"

namespace rat {
    class PP : public sf::Drawable, public sf::Transformable {
    public:
        using Container_t = std::vector<Element*>;

        PP(float radius=15.f, float gap=3.5f);
        ~PP();

        void create(float radius, float gap);

        void update(float deltaTime);

        Element* add(size_t id);

        void remove();

        void randRemove();

        enum ElementType : size_t {
            duch, esencja, fizyczny, ogien, powietrze, woda, ziemia
        };

    private:
        float _radius;
        float _gap;

        Container_t _elements;
        Container_t _trash; 

        Element* _createElement(size_t id) const;

        void _remove(int index);

        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    };
}