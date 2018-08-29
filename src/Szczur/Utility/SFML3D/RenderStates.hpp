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
	RenderStates(Transform transform = Transform(), ShaderProgram* shader = nullptr, Texture* texture = nullptr);
	RenderStates(const RenderStates&) = default;

	Transform transform;
	ShaderProgram* shader;
	const Texture* texture;

	static const RenderStates Default;
};

}
