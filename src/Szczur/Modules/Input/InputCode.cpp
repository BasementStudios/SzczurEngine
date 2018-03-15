#include "InputCode.hpp"

namespace rat
{

InputCode::InputCode() :
	_data(0)
{

}

InputCode::InputCode(Keyboard::Code_e code) :
	_data(InputDevice_e::Keyboard | code)
{

}

InputCode::InputCode(Mouse::Code_e code) :
	_data(InputDevice_e::Mouse | code)
{

}

InputCode::InputCode(Scroll::Code_e code) :
	_data(InputDevice_e::Scroll | code)
{

}

InputCode::InputCode(int data) :
	_data(data)
{

}

InputDevice_e InputCode::getType() const
{
	return static_cast<InputDevice_e>(_data & 0xFF0000);
}

int InputCode::getID() const
{
	return _data & 0xFFFF;
}

InputCode::operator int () const
{
	return getID();
}

int InputCode::getData() const
{
	return _data;
}

bool InputCode::isValid() const
{
	return getData() != 0;
}

}
