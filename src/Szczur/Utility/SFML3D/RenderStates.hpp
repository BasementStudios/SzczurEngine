#pragma once

#include "ShaderProgram.hpp"
#include "Transform.hpp"
//#include <SFML/Graphics.hpp>
#include "Texture.hpp"

namespace sf3d {

	class RenderStates {
	public:
		RenderStates(ShaderProgram* shader = nullptr, Transform transform = Transform(), Texture* texture = nullptr);
		RenderStates(const RenderStates&) = default;


		ShaderProgram* shader;
		Transform transform;
		Texture* texture;
	};
}