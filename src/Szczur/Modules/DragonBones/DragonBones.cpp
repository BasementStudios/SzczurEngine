#include "DragonBones.hpp"

/** @file DragonBones.cpp
 ** @description Implementation file with main class of the World module. 
 ** @author Piotr (H4RRY) <piotrkrupa06@gmail.com>
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

//#include <dragonBones/SF3DFactory.h>

#include "Szczur/Utility/Logger.hpp"

namespace rat 
{

/* Properties */
/// Factory
// DragonBones::Factory_t& DragonBones::getFactory()
// {
// 	return this->factory;
// }
// const DragonBones::Factory_t& DragonBones::getFactory() const
// {
// 	return this->factory;
// }



/* Operators */
// Module constructor/destructor
DragonBones::DragonBones()
{
	LOG_INFO(this, ": DragonBones module initializing");
	this->init();
	LOG_INFO(this, ": DragonBones module initialized");
}
DragonBones::~DragonBones()
{
	LOG_INFO(this, ": DragonBones module destructed");
}



/* Methods */
/// init
void DragonBones::init()
{

}

/// update
void DragonBones::update(float deltaTime)
{
	this->factory.update(deltaTime);
}

}
