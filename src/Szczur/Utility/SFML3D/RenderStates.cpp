#include "RenderStates.hpp"

namespace sf3d {
	class Texture;
	class ShaderProgram;
}
#include "Transform.hpp"

namespace sf3d
{

RenderStates::RenderStates(Transform transform, ShaderProgram* shader, Texture* texture)
:	transform(transform),
	shader(shader),
	texture(texture)
{}

const RenderStates RenderStates::Default {};

}
