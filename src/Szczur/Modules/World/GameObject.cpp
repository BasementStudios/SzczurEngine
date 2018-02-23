#include "GameObject.hpp"

/** @file GameObject.hpp
 ** @description Implementaion file with base object class.
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <string>
#include <vector>

#include "GameObjectType.hpp"

namespace rat
{

/* Properties */
/// Type
const std::string& GameObject::getType() const
{
    return this->type;
}
const std::string& getTypeString() const
{
    return this->type->name;
}

/// State
const StateID_t GameObject::getState() const
{
    return this->stateID;
}
void GameObject::setState(const StateID_t state)
{
    this->stateID = state;
}
const std::string& GameObject::getStateString() const
{
    if (this->type->stateStrings.size() < this->stateID) {
        return this->type->stateStrings[this->stateID];
    } else {
        LOG_WARN("[GameObject:", this, "] State string for ID `", this->stateID, "` was not found; using default.")
        return this->type->stateStrings[0];
    }
}
void GameObject::setState(const std::string& state)
{
    auto& strings = this->type->stateStrings;
    for (std::vector<int>::size_type i = 0; i != strings.size(); i++) {
        if (strings[i] == state) {
            this->setState(i);
            return;
        }
    }
    LOG_WARN("[GameObject:", this, "] State ID for string `", state, "` was not found; using default.");
    this->setState(i);
}

/// Position
GameObject::Vector_t GameObject::getPosition() const 
{
    return this->position;
}
void GameObject::setPosition(const GameObject::Vector_t& position)
{
    this->position = position;
}

/// Speed
GameObject::Vector_t GameObject::getSpeed() const
{
    return this->speed;
}
void GameObject::setSpeed(const GameObject::Vector_t& speed)
{
    this->speed = speed;
}



/* Operators */
/// Constructors/destructor
GameObject(const GameObjectType* type, const std::string& name, const std::string& state, const Vector_t& position, const Vector_t& speed)
{
    this->type = type;
    this->setName(name);
    this->setState(state);
    this->setPosition(position);
    this->setSpeed(speed);
}



/* Methods */
/// update
void GameObject::update(float deltaTime)
{
    ;// @todo . Speed etc.
}

}
