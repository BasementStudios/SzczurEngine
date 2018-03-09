#include "Object.hpp"

/** @file Object.cpp
 ** @description Implementaion file with base object class.
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <string>
#include <vector>

#include "ObjectType.hpp"

namespace rat
{

/* Properties */
/// Type
const ObjectType* Object::getType() const
{
    return this->type;
}
const std::string& Object::getTypeString() const
{
    if (this->type != nullptr) {
        return this->type->name;
    }
    
    const static std::string string(ObjectType::defaultTypeName);
    return string;
}

/// State
const ObjectType::StateID_t Object::getState() const
{
    return this->stateID;
}
void Object::setState(const ObjectType::StateID_t stateID)
{
    this->stateID = stateID;
}
const std::string& Object::getStateString() const
{
    return this->getType()->getStateString(this->getState());
}
void Object::setState(const std::string& stateString)
{
    this->setState(this->getType()->getStateID(stateString));
}

/// Position
Object::Vector_t Object::getPosition() const 
{
    return this->position;
}
void Object::setPosition(const Object::Vector_t& position)
{
    this->position = position;
}

/// Speed
Object::Vector_t Object::getSpeed() const
{
    return this->speed;
}
void Object::setSpeed(const Object::Vector_t& speed)
{
    this->speed = speed;
}



/* Operators */
/// Constructors/destructor
Object::Object(ObjectType* type, const std::string& name, ObjectType::StateID_t state, Object::Vector_t position, Object::Vector_t speed)
{
    this->type = type;
    this->name = name;
    this->setState(state);
    this->setPosition(position);
    this->setSpeed(speed);
}



/* Methods */
void Object::update(float deltaTime)
{
    ;
}

}
