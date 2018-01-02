#pragma once

#include <SFML/Graphics.hpp>

#include "Elements.hpp"

namespace rat {
    class PP : public Elements {
    public:
        PP(float radius=25.f, float gap=5.f);

        void create(float radius, float gap);

        enum ElementType : size_t {
            fire, water, earth, electric
        };

    private:
        Element* _createElement(size_t id) const override;
    };
}