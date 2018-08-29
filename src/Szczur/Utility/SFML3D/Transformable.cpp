#include "Transformable.hpp"

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/trigonometric.hpp> // radians
#include <glm/gtc/matrix_transform.hpp>

#include "Transform.hpp"

namespace sf3d
{

/* Properties */
// Position
void Transformable::setPosition(const glm::vec3 position)
{
	this->position = position;
}
const glm::vec3& Transformable::getPosition() const
{
	return this->position;
}

// Rotation
void Transformable::setRotation(const glm::vec3 direction)
{
	this->rotation = direction;
}
const glm::vec3& Transformable::getRotation() const
{
	return this->rotation;
}

// Scale
void Transformable::setScale(const glm::vec3 value)
{
	this->scalar = value;
}
const glm::vec3& Transformable::getScale() const
{
	return this->scalar;
}

// Origin
void Transformable::setOrigin(const glm::vec3 position)
{
	this->origin.x = position.x;
	this->origin.y = -position.y; // @warn To chyba nie powinno być na tym etapie, bo set(x) =/= x=gett()...
	this->origin.z = position.z;
}
const glm::vec3& Transformable::getOrigin() const
{
	return this->origin;
}



/* Operators */
Transformable::Transformable(
	const glm::vec3& position,
	const glm::vec3& rotation,
	const glm::vec3& scalar,
	const glm::vec3& origin
)
:	position(position),
	rotation(rotation),
	scalar(scalar),
	origin(origin)
{
	;
}



/* Methods */
void Transformable::move(const glm::vec3 offset)
{
	this->position += offset;
}


void Transformable::rotate(const glm::vec3 direction)
{
	this->rotation += direction;
}


void Transformable::scale(const glm::vec3 value)
{
	this->scalar *= value;
}

Transform Transformable::getTransform() const
{
	return Transform(*this);
}

glm::mat4 Transformable::getMatrix() const
{
	glm::mat4 matrix = glm::translate(glm::mat4{1.0f}, this->position);
	matrix = glm::rotate(matrix, glm::radians(this->rotation.x), glm::vec3{ 1.0f, 0.0f, 0.0f });
	matrix = glm::rotate(matrix, glm::radians(this->rotation.y), glm::vec3{ 0.0f, 1.0f, 0.0f });
	matrix = glm::rotate(matrix, glm::radians(this->rotation.z), glm::vec3{ 0.0f, 0.0f, 1.0f });
	matrix = glm::scale(matrix, this->scalar);
	matrix = glm::translate(matrix, -this->origin);

	return matrix;
}

}
