#include "Input.hpp"

namespace rat
{

Input::~Input()
{
    LOG_DESTRUCTOR();
}

void Input::processEvent(const sf::Event& event)
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

void Input::press(const InputCode& code)
{
    switch (code.getType()) {
        case InputDevice_t::Keyboard: _pressKey(code);                break;
        case InputDevice_t::Mouse:    _pressButton(code);             break;
        case InputDevice_t::Scroll:   _moveWheel(code % 2, code - 2); break;
    }
}

void Input::release(const InputCode& code)
{
    switch (code.getType()) {
        case InputDevice_t::Keyboard: _releaseKey(code);              break;
        case InputDevice_t::Mouse:    _releaseButton(code);           break;
        case InputDevice_t::Scroll:   _moveWheel(code % 2, code - 2); break;
    }
}

InputStatus_t Input::getStatus(const InputCode& code) const
{
    switch (code.getType()) {
        case InputDevice_t::Keyboard: return _keyboard[code];
        case InputDevice_t::Mouse:    return _mouse[code];
        case InputDevice_t::Scroll:   return _scroll[code];
    }

    return InputStatus_t::Unkept;
}

bool Input::checkStatus(const InputCode& code, InputStatus_t _Status) const
{
    return getStatus(code) == _Status;
}

bool Input::isUnkept(const InputCode& code) const
{
    return checkStatus(code, InputStatus_t::Unkept);
}

bool Input::isPressed(const InputCode& code) const
{
    return checkStatus(code, InputStatus_t::Pressed);
}

bool Input::isKept(const InputCode& code) const
{
    return checkStatus(code, InputStatus_t::Kept);
}

bool Input::isReleased(const InputCode& code) const
{
    return checkStatus(code, InputStatus_t::Released);
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
    if (_keyboard[id] != InputStatus_t::Kept) {
        _keyboard[id] = InputStatus_t::Pressed;
        _recentlyPressed = Keyboard::Code(id);
    }
}

void Input::_releaseKey(int id)
{
    if (_keyboard[id] != InputStatus_t::Unkept) {
        _keyboard[id] = InputStatus_t::Released;
        _recentlyReleased = Keyboard::Code(id);
    }
}

void Input::_pressButton(int id)
{
    if (_mouse[id] != InputStatus_t::Kept) {
        _mouse[id] = InputStatus_t::Pressed;
        _recentlyPressed = Mouse::Code(id);
    }
}

void Input::_releaseButton(int id)
{
    if (_mouse[id] != InputStatus_t::Unkept) {
        _mouse[id] = InputStatus_t::Released;
        _recentlyReleased = Mouse::Code(id);
    }
}

void Input::_moveWheel(int which, int delta)
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

}
