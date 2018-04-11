#include "BaseObject.hpp"

/** @file BaseObject.cpp
 ** @description Implementaion file with base object class.
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi+basementstudios@gmail.com>
 **/

#include <string>

namespace rat
{

/* Operators */
/// Constructors/destructor
BaseObject::BaseObject()
    : name("")
{}
BaseObject::BaseObject(const std::string& name)
    : name(name)
{}

}
