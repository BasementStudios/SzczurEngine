#include "ArmatureObject.hpp"

/** @file ArmatureObject.cpp
 ** @description Implementation file with armature object class.
 ** @author Piotr (H4RRY) <piotrkrupa06@gmail.com>
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include "Szczur/Modules/World/ObjectType.hpp"
#include "Szczur/Modules/World/Object.hpp"
#include "ArmatureObjectType.hpp"

namespace rat
{

/* Properties */
;



/* Operators */
ArmatureObject(
	const ArmatureObjectType* 	type, 
	const std::string& 			name, 
	Object::Vector_t 			position 	= {0.f, 0.f}, 
	Object::Vector_t 			speed 		= {0.f, 0.f}
) {
	;
}

}
