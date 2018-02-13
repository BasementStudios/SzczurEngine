#pragma once

/** @file Camera.hpp
 ** @description Header file with Camera class. 
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <SFML/System/Vector2.hpp>

namespace rat {

class Camera
{
    /* Types */
public:
    using Position_t = sf::Vector2f;



    /* Fields */
private:
    Position_t* _target;
    Position_t  _position   {0.f, 0.f};



    /* Operators */
public:
    ;
};

}
