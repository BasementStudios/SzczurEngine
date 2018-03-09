#pragma once

/** @file ObjectType.hpp
 ** @description Header file with base of ObjectType class/interface.
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <string>
#include <vector>

namespace rat
{

class ObjectType
{
	/* Types */
public:
	using StateID_t = std::size_t;

	

	/* Constants */
public:
    constexpr static StateID_t  defaultStateID 		{0};
	constexpr static auto       defaultStateString  {"default"};
	constexpr static auto		defaultTypeName		{""};
	


	/* Fields */
public:
	std::string 				name			{defaultTypeName};
protected:
	std::vector<std::string>	statesStrings	{{defaultStateString}};



	/* Properties */
public:
	/// Gets State string/ID.
	const std::string& getStateString(StateID_t stateID) const;
	StateID_t getStateID(const std::string& stateString) const;

};

}
