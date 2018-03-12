#include "Input.hpp"

namespace rat
{

Input::Input()
{
	LOG_INFO(this, " : Module Input constructed");
}

Input::~Input()
{
	LOG_INFO(this, " : Module Input destructed");
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
