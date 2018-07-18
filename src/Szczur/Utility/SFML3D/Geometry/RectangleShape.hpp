#pragma once

/** @file RectangleShape.hpp
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 ** @author Tomasz (Knayder) Jatkowski
 **/

#include "Shape.hpp"

#include <glm/vec2.hpp>

namespace sf3d
{

/// Shape specialization to drawing rectangles
class RectangleShape : public Shape
{
    /* Properties */
public:
    glm::vec2 getSize() const;
    void setSize(glm::vec2 size);



    /* Operators */
public:
    RectangleShape(glm::vec2 size = {0.f, 0.f});
};

}