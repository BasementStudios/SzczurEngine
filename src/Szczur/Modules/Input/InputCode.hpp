#pragma once

#include "Keyboard.hpp"
#include "Mouse.hpp"
#include "Scroll.hpp"

namespace rat
{

enum Input_t { Keyboard = 0x10000, Mouse = 0x20000, Scroll = 0x30000 };

class InputCode
{
private:

	int _data;

public:

	InputCode();

	InputCode(const InputCode&) = default;

	InputCode& operator = (const InputCode&) = default;

	InputCode(InputCode&&) = default;

	InputCode& operator = (InputCode&&) = default;

	InputCode(Keyboard::Code code);

	InputCode(Mouse::Code code);

	InputCode(Scroll::Code code);

	explicit InputCode(int data);

	Input_t getType() const;

	int getID() const;

	operator int () const;

	int getData() const;

	bool isValid() const;

};

}
