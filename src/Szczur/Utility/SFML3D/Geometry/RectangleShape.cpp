#include "RectangleShape.hpp"

/** @file RectanbleShape.cpp
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 ** @author Tomasz (Knayder) Jatkowski
 **/

#include <glm/vec2.hpp>

namespace sf3d
{

glm::vec2 RectangleShape::getSize() const
{
    return {this->vertices[2].position.x, -this->vertices[2].position.y};
}
void RectangleShape::setSize(glm::vec2 size) {
    this->vertices[0].position =  {0.f,      0.f,        0.f};
    this->vertices[1].position =  {size.x,   0.f,        0.f};
    this->vertices[2].position =  {size.x,   -size.y,    0.f};
    this->vertices[3].position =  {0.f,      -size.y,    0.f};
}

RectangleShape::RectangleShape(glm::vec2 size)
:   Shape(4u)
{
    this->setSize(size);
}

}
