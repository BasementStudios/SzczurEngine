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
	
	module.set_function("isKept", [&](Keyboard::Code_e key){return _inputManager.isKept(key);});
	module.set_function("isPressed", [&](Keyboard::Code_e key){return _inputManager.isPressed(key);});
	module.set_function("isUnkept", [&](Keyboard::Code_e key){return _inputManager.isUnkept(key);});

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
