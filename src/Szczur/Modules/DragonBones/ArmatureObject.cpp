#pragma once

/** @file ArmatureObject.cpp
 ** @description Implementation file with armature object class.
 ** @author Piotr (H4RRY) <piotrkrupa06@gmail.com>
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include "Szczur/Modules/World/ObjectType.hpp"
#include "Szczur/Modules/World/ArmatureObjectType.hpp"

namespace rat
{

/* Properties */
;



/* Operators */
ArmatureObject(const ArmatureObjectType* type, const std::string& name, ObjectType::StateID_t stateID = ObjectType::defaultStateID, World::Vector_t position = {0.f, 0.f}, World::Vector_t speed = {0.f, 0.f});

}
