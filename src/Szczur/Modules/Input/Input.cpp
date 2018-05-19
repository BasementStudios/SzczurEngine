#include "Input.hpp"

#include "InputManager.hpp"

namespace rat
{

Input::Input()
{
	LOG_INFO("Initializing Input module");
	Script& script = getModule<Script>();
	auto module = script.newModule("Input");

	module.set("Key_W", Keyboard::Up);
	module.set("Key_A", Keyboard::Left);
	module.set("Key_S", Keyboard::Down);
	module.set("Key_D", Keyboard::Right);
	module.set("Key_Space", Keyboard::Space);

	script.initClasses<InputManager>();

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
