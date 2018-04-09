#include "RenderStates.hpp"

namespace sf3d {

	RenderStates::RenderStates(Shader * shader, Transform transform, Texture * texture) :
		shader(shader),
		transform(transform),
		texture(texture) {}

}