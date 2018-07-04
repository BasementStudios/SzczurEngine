#pragma once

namespace sf3d {
	class Texture;
	class ShaderProgram;
}
#include "Transform.hpp"

namespace sf3d
{

class RenderStates {
public:
	RenderStates(ShaderProgram* shader = nullptr, Transform transform = Transform(), Texture* texture = nullptr);
	RenderStates(const RenderStates&) = default;

	ShaderProgram* shader;
	Transform transform;
	const Texture* texture;

	static const RenderStates Default;
};

}
