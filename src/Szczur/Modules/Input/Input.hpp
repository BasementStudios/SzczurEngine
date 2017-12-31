#pragma once

#include <SFML/Window/Event.hpp>

#include "Szczur/Utility/Modules.hpp"
#include "InputCode.hpp"

namespace rat
{

enum Status_t { Unkept = 0x0, Pressed = 0x1, Kept = 0x2, Released = 0x3 };

class Input : public Module<>, Inputable
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

	bool checkStatus(const InputCode& code, Status_t _Status) const;

	bool isUnkept(const InputCode& code) const;

	bool isPressed(const InputCode& code) const;

	bool isKept(const InputCode& code) const;

	bool isReleased(const InputCode& code) const;

	InputCode getRecentlyPressed() const;

	InputCode getRecentlyReleased() const;

	bool isAnyPressed() const;

	bool isAnyReleased() const;

	bool isTextEntered() const;

	unsigned getEnteredCharacter() const;

private:

	void _pressKey(int id);

	void _releaseKey(int id);

	void _pressButton(int id);

	void _releaseButton(int id);

	void _moveWheel(int which, int delta);

};

}
