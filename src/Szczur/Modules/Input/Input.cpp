#include "Input.hpp"

namespace rat
{

Input::Input()
{
	LOG_INFO("Initializing Input module");
	LOG_INFO("Module Input initialized");
}

Input::~Input()
{
	LOG_INFO("Module Input destructed");
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
