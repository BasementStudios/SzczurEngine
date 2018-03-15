#pragma once

/** @file DragonBones.hpp
 ** @description Header file with main class of the DragonBone module. 
 ** @author Piotr (H4RRY) <piotrkrupa06@gmail.com>
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <memory>

#include <dragonBones/SFMLFactory.h>

#include "Szczur/Modules/World/World.hpp"
#include "Szczur/Utility/Module.hpp"
#include "Szczur/Debug/Logger.hpp"

namespace rat 
{

/** @class DragonBones
 ** @description Provide access to DragonBones armature animation engine.
 **/
class DragonBones : public Module<World>
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
	template <typename ModulesTuple>
	DragonBones(ModulesTuple&& tuple);
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
template <typename ModulesTuple>
inline DragonBones::DragonBones(ModulesTuple&& tuple) : Module(tuple)
{
	LOG_INFO("[DragonBones] Module initializing");
	this->init();
	LOG_INFO("[DragonBones] Module initialized");
}
inline DragonBones::~DragonBones()
{
	LOG_INFO("[DragonBones] Module destructed");
}

}
