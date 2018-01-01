#pragma once

#include <SFML/Window/Event.hpp>

#include "Szczur/Utility/Modules.hpp"
#include "InputCode.hpp"

namespace rat
{

enum Status_t { Unkept = 0x0, Pressed = 0x1, Kept = 0x2, Released = 0x3 };

class Input : public Module<>, Module<>::Inputable
{
	using Module::Module;

private:

	Status_t _keyboard[Keyboard::Count];
	Status_t _mouse[Mouse::Count];
	Status_t _scroll[Scroll::Count];

	InputCode _recentlyPressed;
	InputCode _recentlyReleased;

	unsigned _enteredCharacter;

public:

	void input(const sf::Event& event);

	void finish();

	void press(const InputCode& code);

	void release(const InputCode& code);

	Status_t getStatus(const InputCode& code) const;

	inline bool checkStatus(const InputCode& code, Status_t _Status) const
		{ return getStatus(code) == _Status; }

	inline bool isUnkept(const InputCode& code) const
		{ return checkStatus(code, Status_t::Unkept); }

	inline bool isPressed(const InputCode& code) const
	 	{ return checkStatus(code, Status_t::Pressed); }

	inline bool isKept(const InputCode& code) const
		{ return checkStatus(code, Status_t::Kept); }

	inline bool isReleased(const InputCode& code) const
		{ return checkStatus(code, Status_t::Released); }

	inline InputCode getRecentlyPressed() const
		{ return _recentlyPressed; }

	inline InputCode getRecentlyReleased() const 
		{ return _recentlyReleased; }

	inline bool isAnyPressed() const
		{ return _recentlyPressed.isValid(); }

	inline bool isAnyReleased() const
		{ return _recentlyReleased.isValid(); }

	inline bool isTextEntered() const
		{ return _enteredCharacter != 0; }

	inline unsigned getEnteredCharacter() const
		{ return _enteredCharacter; }

private:

	void _pressKey(int id);

	void _releaseKey(int id);

	void _pressButton(int id);

	void _releaseButton(int id);

	void _moveWheel(int which, int delta);

};

}
