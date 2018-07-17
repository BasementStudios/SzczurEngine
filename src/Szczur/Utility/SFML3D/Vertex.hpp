#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace sf3d
{

struct Vertex
{
	/* Variables */
	glm::vec3 position	{0.f, 0.f, 0.f};
	glm::vec4 color		{1.f, 1.f, 1.f, 1.f};
	glm::vec2 texCoord	{0.f, 0.f};



	/* Operators */
	Vertex() = default;	

	// Copyable
	Vertex(const Vertex& other);
	Vertex& operator = (const Vertex& other);

	Vertex(const glm::vec3& position, const glm::vec4& color, const glm::vec2& texCoord);
};

}
