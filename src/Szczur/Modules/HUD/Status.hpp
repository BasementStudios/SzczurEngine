#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <utility>
#include "Element.hpp"

namespace rat {
    class Status : public sf::Drawable, public sf::Transformable {
    public:
        using Container_t = std::vector<std::pair<size_t, Element*>>;
        using Trash_t = std::vector<Element*>;

        Status(float radius=25.f, float gap = 10.f);
        ~Status();

        void create(float radius, float gap);

        Element* add(size_t id);

        void remove(size_t id);

        void update(float deltaTime);


        enum ElementType : size_t {
            fire, water, wind, electric
        };
    private:
        Container_t _elements;
        Trash_t _trash;

        float _radius;
        float _gap;

        Element* _createElement(size_t id) const;
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    };
}