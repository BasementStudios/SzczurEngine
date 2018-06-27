#pragma once

#include "Shape.hpp"

#include <glm/glm.hpp>

namespace sf3d {
    class CircleShape : public Shape {
    public:
        CircleShape(float radius = 10.f, size_t vertices = 30u);

        void setRadius(float radius);

        void setColor(const glm::vec4& color);
    private:
        virtual glm::vec3 getPointPosition(size_t index) const override;
        virtual glm::vec4 getPointColor(size_t) const override;
        virtual glm::vec2 getPointCoord(size_t) const override;

        float _radius;
        glm::vec4 _color={255.f, 255.f, 255.f, 255.f};
    };
}