#pragma once

#include <array>

#include "Elements.hpp"

namespace rat {
	struct Player {
		std::array<unsigned short, Elements_Size> Elements;
		std::array<unsigned short, 4> Skills;

		Player(std::array<unsigned short, Elements_Size> elements, std::array<unsigned short, 4> skills)
			: Elements(elements), Skills(skills) {
		}
	};
}