#pragma once

/** @file ObjectType.hpp
 ** @description Header file with base of ObjectType class/interface.
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <string>

namespace rat
{

class ObjectType
{
	/* Constants */
public:
    constexpr static auto defaultTypeName = "";
	


	/* Fields */
public:
	std::string name;
};

}
