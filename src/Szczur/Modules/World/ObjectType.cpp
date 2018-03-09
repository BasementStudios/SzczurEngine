#include "ObjectType.hpp"

/** @file ObjectType.cpp
 ** @description Implementaion file with base object type class.
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <string>
#include <vector>

#include "Szczur/Debug/Logger.hpp"
#include "Object.hpp"

namespace rat
{

/// State
const std::string& ObjectType::getStateString(Object::StateID_t stateID) const
{
	if (this->statesStrings.size() > stateID) {
		return this->statesStrings[stateID];
	}
	LOG_WARN("[World][ObjectType{*", this, ";\"", this->name, "}] ",
		"State string for ID `", stateID, "` was not found; using default.");
	return this->statesStrings[ObjectType::defaultStateID];
}
Object::StateID_t ObjectType::getStateID(const std::string& stateString) const
{
    for (std::vector<int>::size_type i = 0, I = this->statesStrings.size(); i != I; i++) {
        if (this->statesStrings[i] == stateString) {
            return i;
        }
    }
    LOG_WARN("[World][ObjectType{*", this, ";\"", this->name, "}] ",
		"State ID for string `", stateString, "` was not found; using default.");
	return ObjectType::defaultStateID;
}

}
