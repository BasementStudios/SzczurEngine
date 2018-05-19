#include "CircleShape.hpp"

namespace sf3d {
    CircleShape::CircleShape(float radius, size_t vertices) :
    _radius{radius},
    Shape{vertices} {
        _update();
    }

    void CircleShape::setRadius(float radius) {
        _radius = radius;
        _update();
    }

    void CircleShape::setColor(const glm::vec4& color) {
        _color = color;
        _update();
    }

    glm::vec3 CircleShape::getPointPosition(size_t index) const {
        static const float pi = 3.141592654f;

        float angle = index * 2 * pi / getVerticesCount() - pi / 2;
        float x = glm::cos(angle) * _radius;
        float y = glm::sin(angle) * _radius;

        return {_radius + x, -_radius + y, 0.f};
    }
    glm::vec4 CircleShape::getPointColor(size_t) const {
        return _color;

    }
    glm::vec2 CircleShape::getPointCoord(size_t) const {
        return {0.f, 0.f};
    }
}