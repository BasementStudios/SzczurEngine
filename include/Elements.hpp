#pragma once

#include "Utility.hpp"

namespace rat {
	enum class Elements : int {
		Fire,
		Water,
		Ground,
		Size
	};

	const auto Elements_Size = enum_v(Elements::Size);
}