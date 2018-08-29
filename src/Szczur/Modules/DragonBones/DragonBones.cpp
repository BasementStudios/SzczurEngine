#include "DragonBones.hpp"

/** @file DragonBones.cpp
 ** @description Implementation file with main class of the DragonBones module. 
 ** @author Piotr (H4RRY) <piotrkrupa06@gmail.com>
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include "Szczur/Utility/Logger.hpp"

namespace rat 
{

/* Operators */
// Module constructor/destructor
DragonBones::DragonBones()
{
	LOG_INFO("Initializing DragonBones module");
	LOG_INFO("Module DragonBones initialized");
}
DragonBones::~DragonBones()
{
	LOG_INFO("Module DragonBones destructed");
}

void DragonBones::update(float deltaTime)
{
	factory.update(deltaTime);
}

}
