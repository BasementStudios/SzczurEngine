#include "Input.hpp"

namespace rat
{

Input::Input()
{
	LOG_INFO(this, " : Input module constructed");
}

Input::~Input()
{
	LOG_INFO(this, " : Input module destructed");
}

InputManager& Input::getManager()
{
	return _inputManager;
}

const InputManager& Input::getManager() const
{
	return _inputManager;
}


}
