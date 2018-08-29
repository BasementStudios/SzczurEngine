#pragma once

/** @file RenderTarget.cpp
 ** @author Tomasz (Knayder) Jatkowski
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include "Shape.hpp"

namespace sf3d
{

/// Shape specialization to drawing circles
class CircleShape : public Shape
{
    /* Vriables */
private:
    float radius;



    /* Properties */
public:
    float getRadius() const;
    void setRadius(float radius);



    /* Operators */
public:
    CircleShape(float radius = 10.f, std::size_t vertices = 30u);
};

}
