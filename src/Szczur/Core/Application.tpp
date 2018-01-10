#pragma once

/** @file Application.cpp
 ** @description Templates implementation file for application main class.
 **/

namespace rat
{

// Module
template<typename TModule>
TModule& Application::getModule()
{
	return this->_modules.getModule<TModule>();
}
template<typename TModule>
const TModule& Application::getModule() const
{
	return this->_modules.getModule<TModule>();
}

}
