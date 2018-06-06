#include "Input.hpp"

#include "InputManager.hpp"

namespace rat
{

#define SET_KEY(KEY) module.set("Key_" #KEY, Keyboard:: KEY);

Input::Input()
{
	LOG_INFO("Initializing Input module");
	Script& script = getModule<Script>();
	auto module = script.newModule("Input");

	SET_KEY(W);	SET_KEY(S);	SET_KEY(A); SET_KEY(D);
	SET_KEY(Up); SET_KEY(Left); SET_KEY(Down); SET_KEY(Right);
	SET_KEY(Space);
	
	SET_KEY(Return);

	// module.set("Key_W", Keyboard::Up);
	// module.set("Key_A", Keyboard::Left);
	// module.set("Key_S", Keyboard::Down);
	// module.set("Key_D", Keyboard::Right);
	// module.set("Key_Space", Keyboard::Space);
	
	module.set_function("isKept", [&](Keyboard::Code_e key){return _inputManager.isKept(key);});
	module.set_function("isPressed", [&](Keyboard::Code_e key){return _inputManager.isPressed(key);});
	module.set_function("isUnkept", [&](Keyboard::Code_e key){return _inputManager.isUnkept(key);});
	module.set_function("isReleased", [&](Keyboard::Code_e key){return _inputManager.isReleased(key);});

	LOG_INFO("Module Input initialized");
}

#undef SET_KEY

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
