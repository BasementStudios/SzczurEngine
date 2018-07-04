#include "RenderStates.hpp"

namespace sf3d {

	RenderStates::RenderStates(ShaderProgram * shader, Transform transform, Texture * texture) :
		shader(shader),
		transform(transform),
		texture(texture) {}
	
	const RenderStates RenderStates::Default {};

}