#include "SceneObject.hpp"

/** @file SceneObject.hpp
 ** @description Implementaion file with standard object class.
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <string>

#include "SpriteDisplayer.hpp" // @test @

namespace rat
{

/* Properties */
/// Type
const std::string& SceneObject::getType() const
{
    return this->_type;
}
void SceneObject::setType(const std::string& type)
{
    this->_type = type;
    // @todo ! displayer odpowiedni dla typu (armature/animatedsprite/sprite)
    this->_displayer = new SpriteDisplayer<SceneObject>(*_assetsStupidPtr, *this); // @todo ! RREEEEEEE
}

/// State
const std::string& SceneObject::getState() const
{
    return this->_state;
}
void SceneObject::setState(const std::string& state)
{
    this->_state = state;
}

/// Position
SceneObject::Vector_t SceneObject::getPosition() const 
{
    return this->_position;
}
void SceneObject::setPosition(const SceneObject::Vector_t& position)
{
    this->_position = position;
}

/// Speed
SceneObject::Vector_t SceneObject::getSpeed() const
{
    return this->_speed;
}
void SceneObject::setSpeed(const SceneObject::Vector_t& speed)
{
    this->_speed = speed;
}



/* Operators */
/// Constructors/destructor
SceneObject::SceneObject()
{
    ;
}
SceneObject::SceneObject(Assets* assetsStupidPtr, const std::string& type, const std::string& state)
{
    this->_assetsStupidPtr = assetsStupidPtr;

    this->setState(state);
    
    // Set type, so create vaild Displayer.
    this->setType(type);
}
SceneObject::~SceneObject()
{
    if (this->_displayer != nullptr)
        delete this->_displayer;
}



/* Methods */
/// update
void SceneObject::update(float deltaTime)
{
    ;// @todo :p np. speed
}

/// draw
void SceneObject::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (this->_displayer != nullptr)
        this->_displayer->draw(*this, target, states);
}

}
