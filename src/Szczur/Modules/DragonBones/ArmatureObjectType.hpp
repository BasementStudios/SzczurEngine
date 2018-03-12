#pragma once

/** @file ArmatureObjectType.hpp
 ** @description Header file with ArmatureObjectType class as shared game armature object type information.
 ** @author Piotr (H4RRY) <piotrkrupa06@gmail.com>
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

;

#include "Szczur/Modules/World/Object.hpp"
#include "Szczur/Modules/World/ObjectType.hpp"

namespace rat
{

class ArmatureObjectType : public ObjectType
{
	/* Fields */
protected:
	;



	/* Properties */
public:
	;



	/* Operators */
public:
	/** @constructor
	 ** @description Loads armature object type.
	 ** @argument name - name of the armature object type to load.
	 **/
	ArmatureObjectType(const std::string& name);
};

}
