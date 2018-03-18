#include "DragonBones.hpp"

/** @file DragonBones.cpp
 ** @description Implementation file with main class of the World module. 
 ** @author Piotr (H4RRY) <piotrkrupa06@gmail.com>
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <dragonBones/SFMLFactory.h>

namespace rat 
{

/* Properties */
/// Factory
DragonBones::Factory_t& DragonBones::getFactory()
{
	return this->factory;
}
const DragonBones::Factory_t& DragonBones::getFactory() const
{
	return this->factory;
}


/* Methods */
/// init
void DragonBones::init()
{
	;
}

/// update
void DragonBones::update(float deltaTime)
{
	this->factory.update(deltaTime);
}

}
