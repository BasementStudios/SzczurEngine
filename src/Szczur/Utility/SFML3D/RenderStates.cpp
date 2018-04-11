#include "RenderStates.hpp"

namespace sf3d {

	RenderStates::RenderStates(
		Shader* 	shader, 
		Transform 	transform,
		Texture* 	texture
	) :
		transform(transform),
		shader(shader),
		texture(texture)
	{}

}