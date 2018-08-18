#include "CircleShape.hpp"

/** @file CircleShape.cpp
 ** @author Tomasz (Knayder) Jatkowski
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <glm/trigonometric.hpp> // cos, sin

namespace sf3d
{

/* Properties */
float CircleShape::getRadius() const
{
    return this->radius;
}
void CircleShape::setRadius(float radius)
{
    const float myFancyNamedVariable = this->getVerticesCount() * 3.141592654f / 2.f;
    this->radius = radius;
    
    for (std::size_t i = 0; i < this->vertices.getSize(); ++i) {
        float angle = (i * (4.f * 3.141592654f)) / myFancyNamedVariable;
        
        float x = glm::cos(angle) * radius;
        float y = glm::sin(angle) * radius;

        this->vertices[i].position = {radius + x, -radius + y, 0.f};
    }
}



/* Operators */
CircleShape::CircleShape(float radius, size_t vertices) 
:   Shape {vertices},
    radius {radius}
{
    ;
}

}
