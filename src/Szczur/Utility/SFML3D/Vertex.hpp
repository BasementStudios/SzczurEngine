#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace sf3d
{

struct Vertex
{
	glm::vec3 position	{0.f, 0.f, 0.f};
	glm::vec4 color		{1.f, 1.f, 1.f, 1.f};
	glm::vec2 texCoord	{0.f, 0.f};
};

}
