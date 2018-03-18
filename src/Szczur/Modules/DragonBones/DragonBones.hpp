#pragma once

/** @file DragonBones.hpp
 ** @description Header file with main class of the DragonBone module. 
 ** @author Piotr (H4RRY) <piotrkrupa06@gmail.com>
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <memory>

#include <dragonBones/SFMLFactory.h>

#include "Szczur/Utility/Modules/Module.hpp"
#include "Szczur/Debug.hpp"

namespace rat 
{

/** @class DragonBones
 ** @description Provide access to DragonBones armature animation engine.
 **/
class DragonBones : public Module<>
{ 
	/* Types */
public:
	using Factory_t = dragonBones::SFMLFactory;
	
	/* Fields */
private:
	// @info Factory use statics inside.
	Factory_t factory;



	/* Properties */
public:
	/** @property Factory
	 ** @description Provides access to armature factory.
	 ** @access referecne get
	 **/
	Factory_t& getFactory();
	const Factory_t& getFactory() const;



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



	/* Methods */
public:
	// Module system
	void init();
	void update(float deltaTime);
};



// Module constructor/destructor
inline DragonBones::DragonBones()
{
	LOG_INFO(this, ": DragonBones module initializing");
	this->init();
	LOG_INFO(this, ": DragonBones module initialized");
}
inline DragonBones::~DragonBones()
{
	LOG_INFO(this, ": DragonBones module destructed");
}

}
