#pragma once

/** @file DragonBones.hpp
 ** @description Header file with main class of the DragonBone module. 
 ** @author Piotr (H4RRY) <piotrkrupa06@gmail.com>
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <memory>

#include "Szczur/Utility/Modules/Module.hpp"

#include "SF3DFactory.hpp"

namespace rat 
{

/** @class DragonBones
 ** @description Provide access to DragonBones armature animation engine.
 **/
class DragonBones : public Module<>
{ 
	/* Fields */
public:
	dragonBones::SF3DFactory factory;

	/* Operators */
public:
	// Module constructor/destructor
	DragonBones();
	~DragonBones();

	// Disable coping
	DragonBones(const DragonBones&) = delete;
	DragonBones& operator = (const DragonBones&) = delete;

	// Disable moving
	DragonBones(DragonBones&&) = delete;
	DragonBones& operator = (DragonBones&&) = delete;

	// Update
	void update(float deltaTime);
};

}
