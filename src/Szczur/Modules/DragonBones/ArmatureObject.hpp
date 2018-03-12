#pragma once

/** @file ArmatureObject.hpp
 ** @description Header file with armature object class.
 ** @author Piotr (H4RRY) <piotrkrupa06@gmail.com>
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include "Szczur/Modules/World/World.hpp"
#include "Szczur/Modules/World/Object.hpp"
#include "Szczur/Modules/World/ObjectType.hpp"
#include "Szczur/Modules/World/ArmatureObjectType.hpp"

namespace rat
{

/** @class ArmatureObject
 ** @implements Object
 **/
class ArmatureObject : public Object
{
	/* Properties */
public:
	/** @property 
	 ** @description
	 ** @access get 
	 **/
	;



	/* Operators */
public:
	ArmatureObject(const ArmatureObjectType* type, const std::string& name, ObjectType::StateID_t stateID = ObjectType::defaultStateID, World::Vector_t position = {0.f, 0.f}, World::Vector_t speed = {0.f, 0.f});



	/* Methods */
public:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override;
}

}
