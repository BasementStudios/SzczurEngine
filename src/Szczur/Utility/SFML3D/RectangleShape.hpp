#pragma once

#include "Shape.hpp"

#include <glm/glm.hpp>

namespace sf3d {
    class RectangleShape : public Shape {
    public:
        RectangleShape(const glm::vec2& size={0.f, 0.f});

        void setSize(const glm::vec2& size);

        void setColor(const glm::vec4& color);


    protected:
        virtual glm::vec3 getPointPosition(size_t index) const override;
        virtual glm::vec4 getPointColor(size_t) const override;
        virtual glm::vec2 getPointCoord(size_t) const override;
        glm::vec2 _size;
        glm::vec4 _color={1.f, 1.f, 1.f, 1.f};
    };
}