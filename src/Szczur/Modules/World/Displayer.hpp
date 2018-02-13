#pragma once

/** @file Displayer.hpp
 ** @description Header file with Displayer interface.
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

namespace rat
{

/** @interface @class Displayer
 ** @desciption Provides function to draw object on the render target. 
 ** @template TObject - type of object to be displayed.
 **/
template <typename TObject>
class Displayer
{
public:
    /** @method draw
     ** @description Draws the object on the render target with the render states.
     ** @virtual pure
     **/
    virtual void draw(const TObject& object, sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const = 0;

    virtual ~Displayer() = 0;
};

template <typename TObject>
inline Displayer<TObject>::~Displayer() {};

}
