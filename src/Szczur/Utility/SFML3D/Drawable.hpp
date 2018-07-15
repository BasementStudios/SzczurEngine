#pragma once
#include "RenderTarget.hpp"
#include "RenderStates.hpp"

namespace sf3d {

	class Drawable {
	public:
		virtual ~Drawable() {};

		virtual void draw(RenderTarget& target, RenderStates states = RenderStates::Default) const = 0;
	};
}