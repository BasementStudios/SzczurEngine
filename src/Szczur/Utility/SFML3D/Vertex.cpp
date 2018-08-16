#include "Vertex.hpp"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace sf3d
{

Vertex::Vertex(const Vertex& other)
:	position(other.position),
	color(other.color),
	texCoord(other.texCoord)
{
	;
}

Vertex& Vertex::operator = (const Vertex& other)
{
	this->position 	= other.position;
	this->color 	= other.color;
	this->texCoord 	= other.texCoord;
    return *this;
}

Vertex::Vertex(const glm::vec3& position, const glm::vec4& color, const glm::vec2& texCoord)
:	position(position),
	color(color),
	texCoord(texCoord)
{
	;
}

}
