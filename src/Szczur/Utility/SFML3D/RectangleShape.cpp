#include "RectangleShape.hpp"


namespace sf3d
{

RectangleShape::RectangleShape(const glm::vec2& size)
:   _size(size)
{
    _update();
}

void RectangleShape::setSize(const glm::vec2& size) {
    _size = size;
    _update();
}

void RectangleShape::setColor(const glm::vec4& color) {
    _color = color;
    _update();
}

glm::vec3 RectangleShape::getPointPosition(size_t index) const {
    switch(index) {
        case 0: return {0.f, 0.f, 0.f};
        case 1: return {_size.x, 0.f, 0.f};
        case 2: return {_size.x, -_size.y, 0.f};
        case 3: return {0.f, -_size.y, 0.f};
    }
    return {0.f, 0.f, 0.f};
}
glm::vec4 RectangleShape::getPointColor(size_t) const {
    return _color;

}
glm::vec2 RectangleShape::getPointCoord(size_t) const {
    return {0.f, 0.f};
}

}
