#include "Input.hpp"

namespace rat
{

void Input::input(const sf::Event& event)
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

void Input::finish()
{
	if (_recentlyPressed.isValid()) {
		switch (_recentlyPressed.getType()) {
			case Input_t::Keyboard: _keyboard[_recentlyPressed] = Status_t::Kept; break;
			case Input_t::Mouse: _mouse[_recentlyPressed] = Status_t::Kept; break;
			case Input_t::Scroll: _scroll[_recentlyPressed] = Status_t::Kept; break;
		}

		_recentlyPressed = InputCode();
	}

	if (_recentlyReleased.isValid()) {
		switch (_recentlyReleased.getType()) {
			case Input_t::Keyboard: _keyboard[_recentlyReleased] = Status_t::Unkept; break;
			case Input_t::Mouse: _mouse[_recentlyReleased] = Status_t::Unkept; break;
			case Input_t::Scroll: _scroll[_recentlyReleased] = Status_t::Unkept; break;
		}

		_recentlyReleased = InputCode();
	}

	_enteredCharacter = 0;
}

void Input::press(const InputCode& code)
{
	switch (code.getType()) {
		case Input_t::Keyboard: _pressKey(code); break;
		case Input_t::Mouse: _pressButton(code); break;
		case Input_t::Scroll: _moveWheel(code % 2, code - 2); break;
	}
}

void Input::release(const InputCode& code)
{
	switch (code.getType()) {
		case Input_t::Keyboard: _releaseKey(code); break;
		case Input_t::Mouse: _releaseButton(code); break;
		case Input_t::Scroll: _moveWheel(code % 2, code - 2); break;
	}
}

Status_t Input::getStatus(const InputCode& code) const
{
	switch (code.getType()) {
		case Input_t::Keyboard: return _keyboard[code];
		case Input_t::Mouse: return _mouse[code];
		case Input_t::Scroll: return _scroll[code];
	}

	return Status_t::Unkept;
}

bool Input::checkStatus(const InputCode& code, Status_t _Status) const
{
	return getStatus(code) == _Status;
}

bool Input::isUnkept(const InputCode& code) const
{
	return checkStatus(code, Status_t::Unkept);
}

bool Input::isPressed(const InputCode& code) const
{
	return checkStatus(code, Status_t::Pressed);
}

bool Input::isKept(const InputCode& code) const
{
	return checkStatus(code, Status_t::Kept);
}

bool Input::isReleased(const InputCode& code) const
{
	return checkStatus(code, Status_t::Released);
}

InputCode Input::getRecentlyPressed() const
{
	return _recentlyPressed;
}

InputCode Input::getRecentlyReleased() const
{
	return _recentlyReleased;
}

bool Input::isAnyPressed() const
{
	return _recentlyPressed.isValid();
}

bool Input::isAnyReleased() const
{
	return _recentlyReleased.isValid();
}

bool Input::isTextEntered() const
{
	return _enteredCharacter != 0;
}

unsigned Input::getEnteredCharacter() const
{
	return _enteredCharacter;
}

void Input::_pressKey(int id)
{
	if (_keyboard[id] != Status_t::Kept) {
		_keyboard[id] = Status_t::Pressed;
		_recentlyPressed = Keyboard::Code(id);
	}
}

void Input::_releaseKey(int id)
{
	if (_keyboard[id] != Status_t::Unkept) {
		_keyboard[id] = Status_t::Released;
		_recentlyReleased = Keyboard::Code(id);
	}
}

void Input::_pressButton(int id)
{
	if (_mouse[id] != Status_t::Kept) {
		_mouse[id] = Status_t::Pressed;
		_recentlyPressed = Mouse::Code(id);
	}
}

void Input::_releaseButton(int id)
{
	if (_mouse[id] != Status_t::Unkept) {
		_mouse[id] = Status_t::Released;
		_recentlyReleased = Mouse::Code(id);
	}
}

void Input::_moveWheel(int which, int delta)
{
	if (which == sf::Mouse::Wheel::VerticalWheel) {
		if (delta < 0) {
			_scroll[Scroll::Up] = Status_t::Released;
			_scroll[Scroll::Down] = Status_t::Pressed;
			_recentlyPressed = Scroll::Code(Scroll::Down);
			_recentlyReleased = Scroll::Code(Scroll::Up);
		}
		else {
			_scroll[Scroll::Up] = Status_t::Pressed;
			_scroll[Scroll::Down] = Status_t::Released;
			_recentlyPressed = Scroll::Code(Scroll::Up);
			_recentlyReleased = Scroll::Code(Scroll::Down);
		}
	}
	else if (which == sf::Mouse::Wheel::HorizontalWheel) {
		if (delta < 0) {
			_scroll[Scroll::Left] = Status_t::Released;
			_scroll[Scroll::Right] = Status_t::Pressed;
			_recentlyPressed = Scroll::Code(Scroll::Right);
			_recentlyReleased = Scroll::Code(Scroll::Left);
		}
		else {
			_scroll[Scroll::Left] = Status_t::Pressed;
			_scroll[Scroll::Right] = Status_t::Released;
			_recentlyPressed = Scroll::Code(Scroll::Left);
			_recentlyReleased = Scroll::Code(Scroll::Right);
		}
	}
}

}
