#pragma once

#include <SFML/Window/Mouse.hpp>

namespace rat
{

struct Mouse
{
	enum Code_e
	{
		Left     = sf::Mouse::Left,
		Middle   = sf::Mouse::Middle,
		Right    = sf::Mouse::Right,
		XButton1 = sf::Mouse::XButton1,
		XButton2 = sf::Mouse::XButton2,

		Count    = sf::Mouse::ButtonCount
	};

};

}
