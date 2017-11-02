#pragma once

#include <SFML/Graphics.hpp>

namespace rat {
	class Layer;
	class Drawable {
	public:
		virtual void draw(Layer& layer) const = 0;
	};
}
