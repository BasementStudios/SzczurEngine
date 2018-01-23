#pragma once

#include <SFML/Window/Event.hpp>

#include "InputCode.hpp"
#include "Szczur/Utility/Module.hpp"

namespace rat
{

enum InputStatus_t { Unkept = 0x0, Pressed = 0x1, Kept = 0x2, Released = 0x3 };

class Input : public Module<>
{
public:

	template <typename Tuple>
	Input(Tuple&& tuple);

	Input(const Input&) = delete;

	Input& operator = (const Input&) = delete;

	Input(Input&&) = delete;

	Input& operator = (Input&&) = delete;

	~Input();

	void processEvent(const sf::Event& event);

	void finish();

	void press(const InputCode& code);

	void release(const InputCode& code);

	InputStatus_t getStatus(const InputCode& code) const;

	bool checkStatus(const InputCode& code, InputStatus_t _Status) const;

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

	InputStatus_t _keyboard[Keyboard::Count];
	InputStatus_t _mouse[Mouse::Count];
	InputStatus_t _scroll[Scroll::Count];

	InputCode _recentlyPressed;
	InputCode _recentlyReleased;

	unsigned _enteredCharacter;

};

template <typename Tuple>
Input::Input(Tuple&& tuple) :
	Module(tuple)
{
	LOG_INFO(this, " : Module Input created");
}

}
