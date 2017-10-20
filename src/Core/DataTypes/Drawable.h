// This class is a interface for any another drawable class
// e.g. Sprite, Armature, Animation...
// Method draw(...) drawing any drawable object on Layer

#pragma once

#include <iostream>
#include <fstream>

#include <SFML/Graphics.hpp>

namespace rat {
	class Layer;
	class Drawable {
	public:
		virtual void draw(Layer& layer) const = 0;
	};
}