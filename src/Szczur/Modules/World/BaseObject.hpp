#pragma once

/** @file BaseObject.hpp
 ** @description Header file with base game object class.
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <string>

#include "Szczur/Utility/SFML3D/Transformable.hpp"

namespace rat
{

/** @class BaseObject
 ** @inherit sf3d::Transformable
 ** @description Base class for in-game objects.
 **/
class BaseObject : public sf3d::Transformable
{
	/* Variables */
public:
	std::string name;
	//#ifdef EDITOR
	bool locked{false};
	//#endif


    /* Operators */
public:
	BaseObject();
	BaseObject(const std::string&  name);
	virtual ~BaseObject() = default;
};

}
