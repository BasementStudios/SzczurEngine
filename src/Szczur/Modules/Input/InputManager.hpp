#pragma once

#include <SFML/Window/Event.hpp>

#include <glm/vec2.hpp>

#include "InputCode.hpp"

namespace rat
{

class Script;
enum InputStatus_e { Unkept = 0x0, Pressed = 0x1, Kept = 0x2, Released = 0x3 };
class Window;

class InputManager
{
public:

	///
	InputManager();

	///
	InputManager(const InputManager&) = delete;

	///
	InputManager& operator = (const InputManager&) = delete;

	///
	InputManager(InputManager&&) = delete;

	///
	InputManager& operator = (InputManager&&) = delete;

	///
	~InputManager() = default;

	///
	void processEvent(const sf::Event& event);

	///
	void finishLogic();

	///
	void press(const InputCode& code);

	///
	void release(const InputCode& code);

	///
	InputStatus_e getStatus(const InputCode& code) const;

	///
	bool checkStatus(const InputCode& code, InputStatus_e status) const;

	///
	bool isUnkept(const InputCode& code) const;

	///
	bool isPressed(const InputCode& code) const;

	///
	bool isKept(const InputCode& code) const;

	///
	bool isReleased(const InputCode& code) const;

	///
	InputCode getRecentlyPressed() const;

	///
	InputCode getRecentlyReleased() const;

	///
	bool isAnyPressed() const;

	///
	bool isAnyReleased() const;

	///
	bool isTextEntered() const;

	///
	unsigned getEnteredCharacter() const;

	///
	sf::Vector2i getMousePosition() const;

	///
	glm::vec2 getWindowMousePosition() const;

	///
	glm::vec2 getScreenMousePosition() const;

private:

	///
	void _pressKey(int id);

	///
	void _releaseKey(int id);

	///
	void _pressButton(int id);

	///
	void _releaseButton(int id);

	///
	void _moveWheel(int which, int delta);

	InputStatus_e _keyboard[Keyboard::Count] = {};
	InputStatus_e _mouse[Mouse::Count] = {};
	InputStatus_e _scroll[Scroll::Count] = {};

	InputCode _recentlyPressed = {};
	InputCode _recentlyReleased = {};

	unsigned _enteredCharacter = 0u;

	glm::vec2 _windowMousePosition = { 0, 0 };
	glm::vec2 _screenMousePosition = { 0, 0 };

	Window *_windowModule = nullptr;
};

}
