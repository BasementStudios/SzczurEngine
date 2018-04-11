#pragma once

#include "Shader.hpp"
#include "Transform.hpp"
//#include <SFML/Graphics.hpp>
#include "Texture.hpp"

namespace sf3d {

	class RenderStates {
	public:
		RenderStates(
			Shader* 	shader 		= nullptr, 
			Transform 	transform 	= Transform(), 
			Texture* 	texture 	= nullptr
		);
		RenderStates(const RenderStates&) = default;

		Transform 		transform	{};
		Shader* 		shader 		{nullptr};
		const Texture* 	texture 	{nullptr};
	};
}