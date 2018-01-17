#pragma once

#include <SFML/Window/Event.hpp>

#include "Szczur/Utility/Modules.hpp"
#include "InputCode.hpp"

namespace rat
{

enum InputStatus_t { Unkept = 0x0, Pressed = 0x1, Kept = 0x2, Released = 0x3 };

class Input : public Module<>
{
public:

	template <typename Tuple>
	Input(Tuple&& tuple) :
		Module(tuple)
	{
		LOG_INFO(this, " -> Module Input created");
	}

	Input(const Input&) = delete;

	Input& operator = (const Input&) = delete;

	Input(Input&&) = delete;

	Input& operator = (Input&&) = delete;

	~Input()
	{
		LOG_INFO(this, " -> Module Input destructed");
	}

	void processEvent(const sf::Event& event)
	{
		if (event.type == sf::Event::KeyPressed) {
			_pressKey(event.key.code);
		}
		else if (event.type == sf::Event::KeyReleased) {
			_releaseKey(event.key.code);
		}
		else if (event.type == sf::Event::MouseButtonPressed) {
			_pressButton(event.mouseButton.button);
		}
		else if (event.type == sf::Event::MouseButtonReleased) {
			_releaseButton(event.mouseButton.button);
		}
		else if (event.type == sf::Event::MouseWheelScrolled) {
			_moveWheel(event.mouseWheelScroll.wheel, event.mouseWheelScroll.delta);
		}
		else if (event.type == sf::Event::TextEntered) {
			_enteredCharacter = event.text.unicode;
		}
	}

	void finish()
	{
		if (_recentlyPressed.isValid()) {
			switch (_recentlyPressed.getType()) {
				case InputDevice_t::Keyboard:   _keyboard[_recentlyPressed] = InputStatus_t::Kept; break;
				case InputDevice_t::Mouse:      _mouse[_recentlyPressed]    = InputStatus_t::Kept; break;
				case InputDevice_t::Scroll:     _scroll[_recentlyPressed]   = InputStatus_t::Kept; break;
			}

			_recentlyPressed = InputCode();
		}

		if (_recentlyReleased.isValid()) {
			switch (_recentlyReleased.getType()) {
				case InputDevice_t::Keyboard: _keyboard[_recentlyReleased] = InputStatus_t::Unkept; break;
				case InputDevice_t::Mouse:    _mouse[_recentlyReleased]    = InputStatus_t::Unkept; break;
				case InputDevice_t::Scroll:   _scroll[_recentlyReleased]   = InputStatus_t::Unkept; break;
			}

			_recentlyReleased = InputCode();
		}

		_enteredCharacter = 0;
	}

	void press(const InputCode& code)
	{
		switch (code.getType()) {
			case InputDevice_t::Keyboard: _pressKey(code);                break;
			case InputDevice_t::Mouse:    _pressButton(code);             break;
			case InputDevice_t::Scroll:   _moveWheel(code % 2, code - 2); break;
		}
	}

	void release(const InputCode& code)
	{
		switch (code.getType()) {
			case InputDevice_t::Keyboard: _releaseKey(code);              break;
			case InputDevice_t::Mouse:    _releaseButton(code);           break;
			case InputDevice_t::Scroll:   _moveWheel(code % 2, code - 2); break;
		}
	}

	InputStatus_t getStatus(const InputCode& code) const
	{
		switch (code.getType()) {
			case InputDevice_t::Keyboard: return _keyboard[code];
			case InputDevice_t::Mouse:    return _mouse[code];
			case InputDevice_t::Scroll:   return _scroll[code];
		}

		return InputStatus_t::Unkept;
	}

	bool checkStatus(const InputCode& code, InputStatus_t _Status) const
	{
		return getStatus(code) == _Status;
	}

	bool isUnkept(const InputCode& code) const
	{
		return checkStatus(code, InputStatus_t::Unkept);
	}

	bool isPressed(const InputCode& code) const
	{
		return checkStatus(code, InputStatus_t::Pressed);
	}

	bool isKept(const InputCode& code) const
	{
		return checkStatus(code, InputStatus_t::Kept);
	}

	bool isReleased(const InputCode& code) const
	{
		return checkStatus(code, InputStatus_t::Released);
	}

	InputCode getRecentlyPressed() const
	{
		return _recentlyPressed;
	}

	InputCode getRecentlyReleased() const
	{
		return _recentlyReleased;
	}

	bool isAnyPressed() const
	{
		return _recentlyPressed.isValid();
	}

	bool isAnyReleased() const
	{
		return _recentlyReleased.isValid();
	}

	bool isTextEntered() const
	{
		return _enteredCharacter != 0;
	}

	unsigned getEnteredCharacter() const
	{
		return _enteredCharacter;
	}

private:

	void _pressKey(int id)
	{
		if (_keyboard[id] != InputStatus_t::Kept) {
			_keyboard[id] = InputStatus_t::Pressed;
			_recentlyPressed = Keyboard::Code(id);
		}
	}

	void _releaseKey(int id)
	{
		if (_keyboard[id] != InputStatus_t::Unkept) {
			_keyboard[id] = InputStatus_t::Released;
			_recentlyReleased = Keyboard::Code(id);
		}
	}

	void _pressButton(int id)
	{
		if (_mouse[id] != InputStatus_t::Kept) {
			_mouse[id] = InputStatus_t::Pressed;
			_recentlyPressed = Mouse::Code(id);
		}
	}

	void _releaseButton(int id)
	{
		if (_mouse[id] != InputStatus_t::Unkept) {
			_mouse[id] = InputStatus_t::Released;
			_recentlyReleased = Mouse::Code(id);
		}
	}

	void _moveWheel(int which, int delta)
	{
		if (which == sf::Mouse::Wheel::VerticalWheel) {
			if (delta < 0) {
				_scroll[Scroll::Up]   = InputStatus_t::Released;
				_scroll[Scroll::Down] = InputStatus_t::Pressed;
				_recentlyPressed  = Scroll::Code(Scroll::Down);
				_recentlyReleased = Scroll::Code(Scroll::Up);
			}
			else {
				_scroll[Scroll::Up]   = InputStatus_t::Pressed;
				_scroll[Scroll::Down] = InputStatus_t::Released;
				_recentlyPressed  = Scroll::Code(Scroll::Up);
				_recentlyReleased = Scroll::Code(Scroll::Down);
			}
		}
		else if (which == sf::Mouse::Wheel::HorizontalWheel) {
			if (delta < 0) {
				_scroll[Scroll::Left]  = InputStatus_t::Released;
				_scroll[Scroll::Right] = InputStatus_t::Pressed;
				_recentlyPressed  = Scroll::Code(Scroll::Right);
				_recentlyReleased = Scroll::Code(Scroll::Left);
			}
			else {
				_scroll[Scroll::Left]  = InputStatus_t::Pressed;
				_scroll[Scroll::Right] = InputStatus_t::Released;
				_recentlyPressed  = Scroll::Code(Scroll::Left);
				_recentlyReleased = Scroll::Code(Scroll::Right);
			}
		}
	}

	InputStatus_t _keyboard[Keyboard::Count];
	InputStatus_t _mouse[Mouse::Count];
	InputStatus_t _scroll[Scroll::Count];

	InputCode _recentlyPressed;
	InputCode _recentlyReleased;

	unsigned _enteredCharacter;

};

}
