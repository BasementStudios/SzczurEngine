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

	SET_KEY(A); SET_KEY(B); SET_KEY(C); SET_KEY(D); SET_KEY(E);
	SET_KEY(F); SET_KEY(G); SET_KEY(H); SET_KEY(I); SET_KEY(J);
	SET_KEY(K); SET_KEY(L); SET_KEY(M); SET_KEY(N); SET_KEY(O);
	SET_KEY(P); SET_KEY(Q); SET_KEY(R); SET_KEY(X); SET_KEY(Y);
	SET_KEY(S); SET_KEY(T); SET_KEY(U); SET_KEY(V); SET_KEY(W);
	SET_KEY(Z);

	SET_KEY(Num0); SET_KEY(Num1); SET_KEY(Num2);
	SET_KEY(Num3); SET_KEY(Num4); SET_KEY(Num5);
	SET_KEY(Num6); SET_KEY(Num7); SET_KEY(Num8);
	SET_KEY(Num9);

	SET_KEY(Escape); SET_KEY(LControl); SET_KEY(LShift); SET_KEY(LAlt); 
	SET_KEY(LSystem); SET_KEY(RControl); SET_KEY(RShift); SET_KEY(RAlt); 
	SET_KEY(RSystem); SET_KEY(Menu); SET_KEY(LBracket); SET_KEY(RBracket);
	SET_KEY(SemiColon); SET_KEY(Comma); SET_KEY(Period); SET_KEY(Quote); 
	SET_KEY(Slash); SET_KEY(BackSlash); SET_KEY(Tilde); SET_KEY(Equal); 
	SET_KEY(Dash); SET_KEY(Space); SET_KEY(Return); SET_KEY(BackSpace); 
	SET_KEY(Tab); SET_KEY(PageUp); SET_KEY(PageDown); SET_KEY(End); 
	SET_KEY(Home); SET_KEY(Insert); SET_KEY(Delete); SET_KEY(Add); 
	SET_KEY(Subtract); SET_KEY(Multiply); SET_KEY(Divide); 
	
	SET_KEY(Left); SET_KEY(Right); SET_KEY(Up); SET_KEY(Down); 
	
	SET_KEY(Numpad0); SET_KEY(Numpad1); SET_KEY(Numpad2);
	SET_KEY(Numpad3); SET_KEY(Numpad4); SET_KEY(Numpad5);
	SET_KEY(Numpad6); SET_KEY(Numpad7); SET_KEY(Numpad8); 
	SET_KEY(Numpad9); 
	
	SET_KEY(F1); SET_KEY(F2); SET_KEY(F3); SET_KEY(F4);
	SET_KEY(F5); SET_KEY(F6); SET_KEY(F7); SET_KEY(F8);
	SET_KEY(F9); SET_KEY(F10); SET_KEY(F11); SET_KEY(F12); 
	SET_KEY(F13); SET_KEY(F14); SET_KEY(F15);
	
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
